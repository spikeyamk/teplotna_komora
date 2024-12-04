#include <ranges>

#include "transceiver.hpp"

namespace admin {
    Transceiver::Transceiver(QObject* parent) :
        QObject(parent),
        serial_port { new QSerialPort(this) },
        nop_timer { new QTimer(this) },
        periodic_timer { new QTimer(this) }
    {
        serial_port->setBaudRate(BAUD_RATE, DIRECTION);
        serial_port->setDataBits(DATA_BITS);
        serial_port->setFlowControl(FLOW_CONTROL);
        serial_port->setParity(PARITY);
        serial_port->setReadBufferSize(static_cast<qint64>(magic::MTU));
        serial_port->setStopBits(STOP_BITS);
        QObject::connect(nop_timer, &QTimer::timeout, this, [this]() {
            if(transmit(CommandResultPair(magic::commands::Nop(), magic::results::Nop())).has_value() == false) {
                disconnect();
            }
        });

        QObject::connect(periodic_timer, &QTimer::timeout, this, [this]() {
            if(periodic_command_result_pair.has_value() == false) {
                disconnect();
                emit error_occured(Error::PeriodicCommandResultIsEmpty);
                return;
            }

            const auto result { transmit(periodic_command_result_pair.value()) };
            if(result.has_value() == false) {
                emit error_occured(Error::CommandTrasmitFailed);
                disconnect();
                return;
            }
            emit result_arrived(result.value());
        });
    }

    bool Transceiver::is_connected() {
        QMutexLocker locker(&mutex);
        return nop_timer->isActive();
    }

    Transceiver::~Transceiver() {
        if(is_connected()) {
            disconnect();
        }
    }

    std::optional<Transceiver::ResultVariant> Transceiver::transmit(const CommandResultPair& command_result) {
        using namespace magic;

        if(std::visit([&](const auto& command) {
            const auto serialized { commands::Serializer::run(command) };
            const qint64 write_ret { serial_port->write(
                reinterpret_cast<const char*>(serialized.data()),
                serialized.size()
            ) };
            if((write_ret < 0)
                || (static_cast<size_t>(write_ret) != serialized.size())
            ) {
                emit error_occured(Error::TransmitWriteWrongSize, QString::number(write_ret));
                return false;
            }
            return true;
        }, std::get<CommandVariant>(command_result)) == false) {
            return std::nullopt;
        }

        if(serial_port->waitForBytesWritten(static_cast<int>(DISCONNECT_TIMEOUT.count())) == false) {
            emit error_occured(Error::TransmitWriteTimeout);
            return std::nullopt;
        }

        while(serial_port->waitForReadyRead(static_cast<int>(TIMEOUT_BETWEEN_BYTES.count()))) {}
        std::array<uint8_t, MTU> read_buf {};
        const qint64 read_ret { serial_port->read(
            reinterpret_cast<char*>(read_buf.data()),
            static_cast<qint64>(read_buf.size())
        ) };

        if((read_ret < 0)
            || (static_cast<size_t>(read_ret) > read_buf.size())
        ) {
            emit error_occured(Error::TransmitResultWrongReadSize, QString::number(read_ret));
            return std::nullopt;
        }

        const std::optional<ResultVariant> result_decoded { results::Deserializer::decode(
            read_buf.begin(),
            read_buf.begin() + static_cast<size_t>(read_ret)
        ) };

        if(result_decoded.has_value() == false) {
            emit error_occured(Error::TransmitResultDecodedNullopt,
                "read_buf: "
                + (std::ranges::fold_left(
                    std::ranges::views::take(read_buf, read_ret)
                    | std::ranges::views::transform([](const uint8_t e) {
                        return QString::asprintf(" 0x%02X,", e);
                    }),
                    QString("{"),
                    [](const QString& lhs, const QString& rhs) {
                        return lhs + rhs;
                    }
                ) | std::ranges::views::reverse | std::ranges::views::drop(1)
                | std::ranges::views::reverse | std::ranges::to<QString>())
                + (read_ret > 0 ? QString(" }") : QString("'empty'"))
            );
            return std::nullopt;
        }

        if(result_decoded.value().index() != std::get<ResultVariant>(command_result).index()) {
            emit error_occured(
                Error::TransmitResultDecodedWrongVariantIndex,
                QString::number(result_decoded.value().index())
                + " against: "
                + QString::number(std::get<ResultVariant>(command_result).index())
            );
            return std::nullopt;
        }

        return result_decoded;
    }

    void Transceiver::connect(const QString port_name) {
        if(is_connected()) {
            emit error_occured(Error::ConnectIsConnected, port_name);
            return;
        }

        serial_port->setBaudRate(BAUD_RATE);
        serial_port->setPortName(port_name);

        if(serial_port->open(QIODeviceBase::ReadWrite) == false) {
            emit error_occured(Error::ConnectSerialPortCantOpen, port_name);
            return;
        }

        if(serial_port->clear() == false) {
            emit error_occured(Error::ConnectSerialPortCantClear, port_name);
            return;
        }

        using namespace magic;
        if(transmit(CommandResultPair(commands::Connect(), results::Connect())).has_value() == false) {
            emit error_occured(Error::ConnectTrasmitFailed, port_name);
            serial_port->close();
            return;
        }

        nop_timer->start(NOP_TIMER_PERIOD);
        emit connected_changed(nop_timer->isActive());
    }

    void Transceiver::disconnect() {
        if(is_connected() == false) {
            emit error_occured(Error::DisconnectIsNotConnected);
            return;
        }

        using namespace magic;
        if(transmit(CommandResultPair(commands::Disconnect(), results::Disconnect())).has_value() == false) {
            emit error_occured(Error::DisconnectTrasmitFailed);
        }

        nop_timer->stop();
        periodic_timer->stop();
        periodic_command_result_pair = std::nullopt;
        serial_port->close();
        emit connected_changed(nop_timer->isActive());
    }

    void Transceiver::one_shot(const CommandResultPair& command_result) {
        if(is_connected() == false) {
            emit error_occured(Error::CommandIsNotConnected);
            return;
        }

        const auto result { transmit(command_result) };
        if(result.has_value() == false) {
            emit error_occured(Error::CommandTrasmitFailed);
            return;
        }

        emit result_arrived(result.value());
    }

    void Transceiver::periodic_start(const CommandResultPair& command_result) {
        if(is_connected() == false) {
            emit error_occured(Error::CommandIsNotConnected);
            return;
        }
        
        periodic_command_result_pair = command_result;
        periodic_timer->start(magic::DISCONNECT_TIMEOUT / 2);
    }
}