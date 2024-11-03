#include <ctime>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "ConsoleHandler.hpp"
#include "SerialHandler.hpp"


namespace fs = std::filesystem;
using json = nlohmann::json;

const std::string ConsoleHandler::sensorReadingsDirectoryPath = "sensor_readings/";
const std::string ConsoleHandler::usageFilePath = "usage.txt";
const std::string ConsoleHandler::logsFilePath = "logs.txt";
const std::string ConsoleHandler::outputDirectoryPath = "output/";
bool  ConsoleHandler::exitApp = false;

ConsoleHandler::ConsoleHandler() {
    // Create the output directory if it does not exist
    if (!fs::exists(outputDirectoryPath)) {
        fs::create_directories(outputDirectoryPath);
        CreateFileIfNotExists(logsFilePath);
        exitApp = false;
    }
}

ConsoleHandler::~ConsoleHandler() {
    if (exitApp == false) {
        Exit();
    }
}

void ConsoleHandler::Run() {
    PrintMessage("Welcome to temperature regulator admin!");
    PrintUsage();

    while (exitApp == false)
    {
        std::string command;
        std::cout << "Enter a command: ";
        std::cin >> command;

        if (command == "read-all") {
            ReadAll();
        } else if (command == "set-temperature") {
            float temperature;
            std::cout << "Enter the temperature to set: ";
            std::cin >> temperature;
            SetTemperature(temperature);
        } else if (command == "export") {
            std::string destinationDirectoryPath;
            std::cout << "Enter the destination directory path: ";
            std::cin >> destinationDirectoryPath;
            ExportOutput(destinationDirectoryPath);
        } else if (command == "exit") {
            Exit();
        } else {
            PrintError("Error: Invalid command. Please try again.");
            PrintUsage();
        }
    }
}

void ConsoleHandler::ExportOutput(std::string destinationDirectoryPath) {
    try {
            // Check if the source directory exists
            if (!fs::exists(destinationDirectoryPath) || !fs::is_directory(outputDirectoryPath)) {
                return PrintError("Error: Source directory \"" + destinationDirectoryPath + "\" does not exist or is not a directory.");
            }

            // Create the destination directory if it does not exist
            if (!fs::exists(destinationDirectoryPath)) {
                fs::create_directories(destinationDirectoryPath);
            }

            // Iterate over each file in the source directory
            for (const auto& entry : fs::directory_iterator(outputDirectoryPath)) {
                if (entry.is_regular_file()) { // Check if the entry is a regular file
                    fs::path sourceFile = entry.path();
                    fs::path destinationFile = fs::path(outputDirectoryPath) / sourceFile.filename();

                    // Copy the file from source to destination
                    fs::copy_file(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
                    PrintSuccess("Copied: " + sourceFile.string() + " to " + destinationFile.string());
                }
            }
        } catch (const fs::filesystem_error& e) {
            PrintError("Filesystem error: " + std::string(e.what()));
        } catch (const std::exception& e) {
            PrintError("Error: " + std::string(e.what()));
        }
}

void ConsoleHandler::ReadAll() {
    PrintMessage("Reading all sensor states\n");
    std::string outputFileName = GenerateOutputFilePath();
    AppendSensorReading(25.0); // Dummy sensor reading
    PrintSuccess("All sensor states read succesfully!\n");
    RecordAppLogs("All sensor states read and recorded at " + outputFileName);
}

void ConsoleHandler::SetTemperature(float temperature) {
    PrintMessage("Setting temperature to " + std::to_string(temperature) + " degrees Celsius...\n");
    PrintSuccess("Temperature has been set successfully!\n");
    RecordAppLogs("Temperature set to " + std::to_string(temperature) + " degrees Celsius.");
}

void ConsoleHandler::Exit() {
    PrintMessage("Exiting the application...\n");
    exitApp = true;
    PrintSuccess("Application has been closed successfully!\n");
}

void ConsoleHandler::AppendSensorReading(float temperature) {
    // Generate the file name for the current day
    std::string fileName = GenerateOutputFilePath();
    CreateFileIfNotExists(fileName);

    // Create or load the existing JSON data
    json sensorData;
    std::ifstream inputFile(fileName);
    if (inputFile.is_open()) {
        // If the file exists, parse the JSON data
        inputFile >> sensorData;
    } else {
        // If the file does not exist, initialize as an empty array
        sensorData = json::array();
    }
    inputFile.close();

    // Create a new JSON object for the sensor reading
    json newReading = {
        {"time", GetTime()},
        {"temperature", temperature}
    };

    // Append the new reading to the JSON array
    sensorData.insert(sensorData.begin(), newReading);

    // Save the updated JSON data back to the file
    std::ofstream outputFile(fileName);
    if (outputFile.is_open()) {
        outputFile << sensorData.dump(4); // Pretty print with an indent of 4 spaces
    }
    outputFile.close();
}

std::string ConsoleHandler::GenerateOutputFilePath() {
    // Get the current time
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    // Format the date as YYYY_MM_DD
    std::ostringstream oss;
    oss << outputDirectoryPath
        << sensorReadingsDirectoryPath
        << (now->tm_year + 1900) << "_"
        << (now->tm_mon + 1) << "_"
        << now->tm_mday
        << "_Sensor_Readings.json";

    return oss.str();
}

void ConsoleHandler::SetConsoleColor(const std::string& color) {
    if (color == "RED") {
        std::cout << "\033[31m"; // ANSI escape code for red
    } else if (color == "GREEN") {
        std::cout << "\033[32m"; // ANSI escape code for green
    } else if (color == "NORMAL") {
        std::cout << "\033[0m";  // ANSI escape code for reset/default color
    }
}

void ConsoleHandler::PrintColoredLine(const std::string& line) {
     if (line.find("[RED]") == 0) {
        SetConsoleColor("RED");
        std::cout << line.substr(8) << std::endl; // Skip the marker
    } else if (line.find("[GREEN]") == 0) {
        SetConsoleColor("GREEN");
        std::cout << line.substr(7) << std::endl; // Skip the marker
    } else if (line.find("[NORMAL]") == 0) {
        SetConsoleColor("NORMAL");
        std::cout << line.substr(8) << std::endl; // Skip the marker
    } else {
        // Default color for lines without markers
        SetConsoleColor("NORMAL");
        std::cout << line << std::endl;
    }
}

void ConsoleHandler::CreateFileIfNotExists(std::string filePath) {
    // Use std::filesystem to extract the directory path from the full file path
    fs::path path(filePath);

    // Extract the parent directory from the file path
    fs::path parentDir = path.parent_path();

    // Check if the parent directory exists; if not, create it
    if (!fs::exists(parentDir)) {
        if (!fs::create_directories(parentDir)) {
            std::cerr << "Error: Failed to create directories: " << parentDir << std::endl;
            return;
        }
    }

    // Now, create the file if it does not exist
    std::ofstream file(filePath, std::ios::app); // Open the file in append mode to create it if it doesn't exist
    if (!file) {
        std::cerr << "Error: Failed to create the file: " << filePath << std::endl;
        return;
    }

     // Write an empty JSON array to the file
    json emptyArray = json::array();
    file << emptyArray.dump(4); // Format the output with an indentation of 4 spaces
    file.close(); // Close the file after creating it
}

std::string ConsoleHandler::GenerateLogLine(const std::string& message) {
        // Get the current time
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        // Format the date and time as YYYY_MM_DD_HH:MM:SS
        std::ostringstream oss;
        oss << (now->tm_year + 1900) << "_"
            << (now->tm_mon + 1) << "_"
            << now->tm_mday << "_"
            << (now->tm_hour < 10 ? "0" : "") << now->tm_hour << ":"
            << (now->tm_min < 10 ? "0" : "") << now->tm_min << ":"
            << (now->tm_sec < 10 ? "0" : "") << now->tm_sec
            << " " << message;

        return oss.str();
}

void ConsoleHandler::RecordAppLogs(const std::string& data) {
    // Open the file in append mode (std::ios::app)
    std::ofstream file(logsFilePath, std::ios::app);
    if (!file) {
        return PrintError("Error: Unable to record the application logs at " + logsFilePath);
    }

    // Write the data to the file
    file << GenerateLogLine(data) << std::endl;

    // Close the file
    file.close();
}

void ConsoleHandler::WriteData(std::string filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        return PrintError("Error: Unable to open file " + filename + " for writing.");
    }

    // Write the data to the file
    file << data;

    // Close the file
    file.close();
    PrintSuccess("Data written successfully to " + filename);
}

void ConsoleHandler::PrintMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleHandler::PrintError(const std::string& message) {
    std::cout << "\033[31m" << message << "\033[0m" << std::endl;
}

void ConsoleHandler::PrintSuccess(const std::string& message) {
    std::cout << "\033[32m" << message << "\033[0m" << std::endl;
}

void ConsoleHandler::PrintUsage() {
    std::ifstream file(usageFilePath); // Open the file for reading

    if (!file) {
        PrintError("Error: Unable to open file " + usageFilePath);
        return;
    }

    std::string line;
    while (std::getline(file, line)) { // Read file line by line
        PrintColoredLine(line);
    }

    file.close(); // Close the file
}

std::string ConsoleHandler::GetTime() {
    std::time_t t = std::time(nullptr);
    std::tm* localTime = std::localtime(&t);
    std::ostringstream oss;
    oss << localTime->tm_hour << ":"
        << localTime->tm_min << ":"
        << localTime->tm_sec;
    return oss.str();
}