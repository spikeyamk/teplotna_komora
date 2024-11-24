#pragma once

#include <utility>

#include <QSerialPort>
#include <QTimer>
#include <QMutex>

#include "magic/magic.hpp"

namespace admin {
    class Transceiver : public QObject {
        Q_OBJECT
    private:
        static constexpr QSerialPort::BaudRate BAUD_RATE { QSerialPort::Baud115200 };
        static constexpr QSerialPort::DataBits DATA_BITS { QSerialPort::Data8 };
        static constexpr QSerialPort::Direction DIRECTION { QSerialPort::AllDirections };
        static constexpr QSerialPort::FlowControl FLOW_CONTROL { QSerialPort::NoFlowControl };
        static constexpr QSerialPort::Parity PARITY { QSerialPort::NoParity };
        static constexpr QSerialPort::PinoutSignal PINOUT_SIGNAL { QSerialPort::NoSignal };
        static constexpr QSerialPort::StopBits STOP_BITS { QSerialPort::OneStop };
        static constexpr int TIMEOUT_BETWEEN_BYTES_MS { static_cast<int>(magic::TIMEOUT_MS) / 10 };
        static constexpr std::chrono::milliseconds NOP_TIMER_PERIOD { magic::TIMEOUT_MS / 2U };
    private:
        QSerialPort* serial_port;
        QTimer* nop_timer;
        QMutex mutex;
    public:
        using CommandVariant = magic::commands::Deserializer::Variant;
        using ResultVariant = magic::results::Deserializer::Variant;
        using CommandResultPair = std::pair<CommandVariant, ResultVariant>;

        enum class Error {
            ConnectIsConnected,
            ConnectSerialPortCantOpen,
            ConnectSerialPortCantClear,
            ConnectTrasmitFailed,

            DisconnectIsNotConnected,
            DisconnectTrasmitFailed,

            NopTimerTrasmitFailed,

            CommandIsNotConnected,
            CommandTrasmitFailed,

            TransmitWriteWrongSize,
            TransmitWriteTimeout,
            TransmitReadResultTimeout,
            TransmitResultWrongReadSize,
            TransmitResultDecodedNullopt,
            TransmitResultDecodedWrongVariantIndex,
        }; Q_ENUM(Error)
    public:
        Transceiver(QObject* parent = nullptr);
        ~Transceiver();
        bool is_connected();
    private:
        std::optional<ResultVariant> transmit(const CommandResultPair& command_result);
    public slots:
        void connect(const QString port_name);
        void disconnect();
        void one_shot(const CommandResultPair& command_result);
    signals:
        void error_occured(const Error error, const QString message = "");
        void connected_changed(bool connected);
        void result_arrived(const ResultVariant result);
    };
}