#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <string>
#include <ceserial.h>
#include <cstdint>


class SerialHandler {
public:
    SerialHandler();
    ~SerialHandler();

    // Initialize the serial port
    bool initialize();

    // Send a message to the STM32
    void sendMessage(const std::string& message);

    // Receive a message from the STM32
    std::string receiveMessage(uint8_t size);

    // Delay for a given number of milliseconds
    void delay(unsigned long ms);

    // Close the serial port
    bool close();

private:
    int serialPort; // File descriptor for the serial port
};

#endif // SERIALHANDLER_H
