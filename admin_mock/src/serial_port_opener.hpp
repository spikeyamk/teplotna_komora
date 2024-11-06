#pragma once

#include <QSerialPort>

class SerialPortOpener {
private:
    QSerialPort& serial_port;
public:
    SerialPortOpener(QSerialPort& serial_port);
    ~SerialPortOpener();
public:
    bool is_open() const;
};