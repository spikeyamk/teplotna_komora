#include <QDebug>
#include <QMutexLocker>
#include <QByteArray>
#include <QSerialPort>
#include <QVector>

#include "util.hpp"
#include "transceiver.hpp"

Transceiver::Transceiver(QObject* parent) :
    QThread { parent }
{}

Transceiver::~Transceiver() {
    stop_token = true;
    wait_condition.notify_one();
    wait();
}

void Transceiver::run() {
    while(stop_token == false) {
        const QMutexLocker mutex_locker { &mutex };
        wait_condition.wait(&mutex);
        if(stop_token == true) {
            return;
        }

        QSerialPort serial_port { serial_port_info };
        serial_port.setBaudRate(QSerialPort::Baud115200);

        if(serial_port.open(QIODevice::ReadWrite) == false) {
            emit this->error_occured(Error::CantOpen);
            continue;
        }

        if(serial_port.clear() == false) {
            emit this->error_occured(Error::CantClear);
            continue;
        }

        const auto connect_serialized { common::magic::commands::Serializer::run(common::magic::commands::Connect()) };
        if(serial_port.write(reinterpret_cast<const char*>(connect_serialized.data()), connect_serialized.size()) != static_cast<qint64>(connect_serialized.size())) {
            emit this->error_occured(Error::ConnectWriteWrongSize);
            continue;
        }

        if(serial_port.waitForBytesWritten(TIMEOUT_MS) == false) {
            emit this->error_occured(Error::ConnectWriteTimeout);
            continue;
        }

        if(serial_port.waitForReadyRead(TIMEOUT_MS) == false) {
            emit this->error_occured(Error::ConnectReadResultTimeout);
            continue;
        }

        QByteArray connect_result_buf { serial_port.readAll() };
        while(serial_port.waitForReadyRead(TIMEOUT_BETWEEN_BYTES_MS))
            connect_result_buf += serial_port.readAll();
        if(connect_result_buf.size() == 0) {
            emit this->error_occured(Error::ConnectResultBufEmpty);
            continue;
        }

        const QVector<uint8_t> connect_result_vector(connect_result_buf.begin(), connect_result_buf.end());

        const auto connect_result_decoded { common::magic::results::Deserializer::decode(connect_result_vector.begin(), connect_result_vector.end()) };
        if(connect_result_decoded.has_value() == false) {
            emit this->error_occured(Error::ConnectResultDecodedNullopt);
            continue;
        }

        if(is_same<common::magic::results::Connect>(connect_result_decoded.value()) == false) {
            emit this->error_occured(Error::ConnectResultDecodedWrongType);
            continue;
        }

        const auto command_write_visitor = [&]() {
            bool command_write_visitor_success { false };
            std::visit(
                [&](auto&& command) {
                    const auto command_serialized { common::magic::commands::Serializer::run(command) };
                    if(serial_port.write(reinterpret_cast<const char*>(command_serialized.data()), command_serialized.size()) != static_cast<qint64>(command_serialized.size())) {
                        emit this->error_occured(Error::CommandWriteWrongSize);
                        command_write_visitor_success = false;
                        return;
                    }

                    if(serial_port.waitForBytesWritten(TIMEOUT_MS) == false) {
                        emit this->error_occured(Error::CommandWriteTimeout);
                        command_write_visitor_success = false;
                        return;
                    }

                    command_write_visitor_success = true;
                },
                command_result.command_variant
            );
            return command_write_visitor_success;
        };

        if(command_write_visitor() == false) {
            continue;
        }

        if(serial_port.waitForReadyRead(TIMEOUT_MS) == false) {
            emit this->error_occured(Error::CommandReadResultTimeout);
            continue;
        }

        QByteArray command_result_buf { serial_port.readAll() };
        while(serial_port.waitForReadyRead(TIMEOUT_BETWEEN_BYTES_MS))
            command_result_buf += serial_port.readAll();
        if(command_result_buf.size() == 0) {
            emit this->error_occured(Error::CommandResultBufEmpty);
            continue;
        }
        const QVector<uint8_t> command_result_vector(command_result_buf.begin(), command_result_buf.end());
        const auto command_result_decoded { common::magic::results::Deserializer::decode(command_result_vector.begin(), command_result_vector.end()) };

        if(command_result_decoded.has_value() == false) {
            emit this->error_occured(Error::CommandResultDecodedNullopt);
            continue;
        }

        if(command_result.result_variant.index() != command_result_decoded.value().index()) {
            emit this->error_occured(Error::CommandResultDecodedWrongType);
            continue;
        }

        const auto disconnect_serialized { common::magic::commands::Serializer::run(common::magic::commands::Disconnect()) };
        if(serial_port.write(reinterpret_cast<const char*>(disconnect_serialized.data()), disconnect_serialized.size()) != static_cast<qint64>(disconnect_serialized.size())) {
            emit this->error_occured(Error::DisconnectWriteWrongSize);
            continue;
        }

        if(serial_port.waitForBytesWritten(TIMEOUT_MS) == false) {
            emit this->error_occured(Error::DisconnectWriteTimeout);
            continue;
        }

        if(serial_port.waitForReadyRead(TIMEOUT_MS) == false) {
            emit this->error_occured(Error::DisconnectReadResultTimeout);
            continue;
        }

        QByteArray disconnect_result_buf { serial_port.readAll() };
        while(serial_port.waitForReadyRead(TIMEOUT_BETWEEN_BYTES_MS))
            disconnect_result_buf += serial_port.readAll();
        if(disconnect_result_buf.size() == 0) {
            emit this->error_occured(Error::DisconnectResultBufEmpty);
            continue;
        }
        const QVector<uint8_t> disconnect_result_vector(disconnect_result_buf.begin(), disconnect_result_buf.end());

        const auto disconnect_result_decoded { common::magic::results::Deserializer::decode(disconnect_result_vector.begin(), disconnect_result_vector.end()) };
        if(disconnect_result_decoded.has_value() == false) {
            emit this->error_occured(Error::DisconnectResultDecodedNullopt);
            continue;
        }

        if(is_same<common::magic::results::Disconnect>(disconnect_result_decoded.value()) == false) {
            emit this->error_occured(Error::DisconnectResultDecodedWrongType);
            continue;
        }

        emit this->result(command_result_decoded.value());
        emit this->result_buf(command_result_buf);
    }
}

void Transceiver::transmit(const QSerialPortInfo& device, const CommandResult& in_command_variant) {
    const QMutexLocker mutex_locker { &mutex };
    serial_port_info = device;
    command_result = in_command_variant;
    wait_condition.notify_one();
}