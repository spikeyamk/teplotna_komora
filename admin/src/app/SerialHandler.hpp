#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <string>
#include <ceserial.h>
#include <cstdint>


class SerialHandler {
public:
    SerialHandler();
    ~SerialHandler();

    bool Initialize();

    // Send a message to the STM32
    void SendMessage(const std::string& message);

    // Receive a message from the STM32
    std::string ReceiveMessage(uint8_t size);

    // Delay for a given number of milliseconds
    void Delay(unsigned long ms);

    // Close the serial port
    bool close();

private:
    int serialPort; // File descriptor for the serial port
};

#endif // SERIALHANDLER_H
