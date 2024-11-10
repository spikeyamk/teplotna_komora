#include "serial_port_opener.hpp"

SerialPortOpener::SerialPortOpener(QSerialPort& serial_port) :
    serial_port { serial_port }
{
    serial_port.open(QIODevice::ReadWrite);
}

SerialPortOpener::~SerialPortOpener() {
    if(serial_port.isOpen()) {
        serial_port.close();
    }
}

bool SerialPortOpener::is_open() const {
    return serial_port.isOpen();
}