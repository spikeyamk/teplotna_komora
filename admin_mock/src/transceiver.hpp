#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSerialPortInfo>

#include "common/magic/magic.hpp"

class Transceiver : public QThread {
    Q_OBJECT
private:
    bool stop_token { false };
    QMutex mutex {};
    QWaitCondition wait_condition {};
    QSerialPortInfo serial_port_info {};
    static constexpr int TIMEOUT_MS { common::magic::TIMEOUT_MS };
    static constexpr int TIMEOUT_BETWEEN_BYTES_MS { 10 };
public:
    using CommandVariant = common::magic::commands::Deserializer::Variant;
    using ResultVariant = common::magic::results::Deserializer::Variant;

    struct CommandResult {
        CommandVariant command_variant;
        ResultVariant result_variant;
    };

    enum class Error {
        CantOpen,
        CantClear,

        ConnectWriteWrongSize,
        ConnectWriteTimeout,
        ConnectReadResultTimeout,
        ConnectResultBufEmpty,
        ConnectResultDecodedNullopt,
        ConnectResultDecodedWrongType,

        CommandWriteWrongSize,
        CommandWriteTimeout,
        CommandResultDecodedWrongType,
        CommandReadResultTimeout,
        CommandResultBufEmpty,
        CommandResultDecodedNullopt,

        DisconnectWriteWrongSize,
        DisconnectWriteTimeout,
        DisconnectReadResultTimeout,
        DisconnectResultBufEmpty,
        DisconnectResultDecodedNullopt,
        DisconnectResultDecodedWrongType,
    };
    Q_ENUM(Error)
private:
    CommandResult command_result {};
public:
    Transceiver(QObject* parent = nullptr);
    ~Transceiver();
public:
    void transmit(const QSerialPortInfo& device, const CommandResult& in_command_result);
signals:
    void error_occured(const Error error);
    void result(const common::magic::results::Deserializer::Variant& data);
    void result_buf(const QByteArray& buf);
private:
    void run() override;
};