#include "ConsoleHandler.hpp"
#include <iostream>
#include <fstream>
#include "SerialHandler.hpp"

const std::string ConsoleHandler::sensorReadingsDirectoryPath = "sensor_readings/";
const std::string ConsoleHandler::usageFilePath = "usage.txt";
const std::string ConsoleHandler::logsFilePath = "logs.txt";
const std::string ConsoleHandler::outputDirectoryPath = "output/";

void ConsoleHandler::Run() {
    return;
}

void ConsoleHandler::ConfigureOutputDirectory(std::string outputDirectoryPath) {
    return;
}

void ConsoleHandler::ReadAll(std::string outputFileName) {
    return;
}

void ConsoleHandler::SetTemperature(float temperature) {
    return;
}

void ConsoleHandler::Exit() {
    return;
}

std::string ConsoleHandler::GenerateOutputFileName() {
    return "";
}

void ConsoleHandler::WriteData(std::string filename) {
    return;
}

void ConsoleHandler::PrintMessage(const std::string& message) {
    return;
}

void ConsoleHandler::PrintError(const std::string& message) {
    return;
}

void ConsoleHandler::PrintSuccess(const std::string& message) {
    return;
}

void ConsoleHandler::PrintUsage() {
    return;
}