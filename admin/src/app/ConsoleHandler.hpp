// Print usage of the terminal commands
// Read all - Takses a json filename as input, reads data through USART and saves it to the file
// Set temperature - Takes a temperature as input and sends it through USART, Prints whether the temperature was set successfully
// Exit - Exits the program
#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <string>
#include <cstdint>


class ConsoleHandler {
public:
    ConsoleHandler() = delete;
    ~ConsoleHandler() = delete;

    void                        Run();
    void                        ConfigureOutputDirectory(std::string outputDirectoryPath);
    void                        ReadAll(std::string outputFileName);
    void                        SetTemperature(float temperature);
    void                        Exit();
private:
    std::string                 GenerateOutputFileName();
    void                        WriteData(std::string filename);
    void                        PrintMessage(const std::string& message);
    void                        PrintError(const std::string& message);
    void                        PrintSuccess(const std::string& message);
    void                        PrintUsage();

    static const std::string    sensorReadingsDirectoryPath;
    static const std::string    usageFilePath;
    static const std::string    logsFilePath;
    static const std::string    outputDirectoryPath;
};
#endif // SERIALHANDLER_H