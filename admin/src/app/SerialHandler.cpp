#include "SerialHandler.hpp"
#include <iostream>

extern ceSerial com;

SerialHandler::SerialHandler() : serialPort(-1) {}

SerialHandler::~SerialHandler() {
    if (serialPort != -1) {
        com.Close();
    }
}

bool SerialHandler::initialize() {
    std::printf("Opening port %s.\n", com.GetPort().c_str());

	if (com.Open() == 0) {
		std::printf("Listening on port %s.\n", com.GetPort().c_str());
        this->serialPort = 0;
        return true;
    }
	else {
		std::printf("Error opening port %s.\n", com.GetPort().c_str());
        this->serialPort = -1;
		return false;
	}
}

void SerialHandler::sendMessage(const std::string& message) {
    if (serialPort != -1) {
        std::printf("\n\nSending: %s", message.c_str());

        bool dataSent = com.Write(message.c_str());

        if (dataSent) {
            std::printf("Succesfully sent: %s", message.c_str());
        }
        else {
            std::printf("Failed sending: %s", message.c_str());
        }
    } else {
        std::cerr << "Error: Serial port not initialized" << std::endl;
    }
}

std::string SerialHandler::receiveMessage(uint8_t size) {
    printf("Received: ");

    for(uint8_t i = 0; i < size; i++) {
        bool successFlag = false;
        com.Delay(100);
        char c = com.ReadChar(successFlag); // read a char

        if(!successFlag || c == '\n') {
            return "";
        }else {
            std::printf("%c", c);
        }
    }
    return "";
}

void SerialHandler::delay(unsigned long ms) {
    if (serialPort != -1) {
        com.Delay(ms);
    } else {
        std::cerr << "Error: Serial port not initialized" << std::endl;
    }
}

bool SerialHandler::close() {
    if (serialPort != -1) {
        com.Close();
        serialPort = -1;
        return true;
    }
    return false;
}