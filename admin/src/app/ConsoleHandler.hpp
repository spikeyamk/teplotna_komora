#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <string>
#include <cstdint>


class ConsoleHandler {
public:
    ConsoleHandler();
    ~ConsoleHandler();

    void                        Run();
    void                        ExportOutput(std::string outputDirectoryPath);
    void                        ReadAll();
    void                        SetTemperature(float temperature);
    void                        Exit();
private:
    void                        HandleSetTemperature();
    void                        HandleExport();
    void                        HandleInvalidCommand();
    void                        AppendSensorReading(float temperature);
    std::string                 GetTime();
    std::string                 GenerateOutputFilePath();
    void                        SetConsoleColor(const std::string& color);
    void                        PrintColoredLine(const std::string &line);
    void                        CreateFileIfNotExists(std::string filePath);
    void                        WriteData(std::string filename, const std::string& data);
    std::string                 GenerateLogLine(const std::string &message);
    void                        RecordAppLogs(const std::string& data);
    void                        PrintMessage(const std::string& message);
    void                        PrintError(const std::string& message);
    void                        PrintSuccess(const std::string& message);
    void                        PrintUsage();

    static const std::string    sensorReadingsDirectoryPath;
    static const std::string    usageFilePath;
    static const std::string    logsFilePath;
    static const std::string    outputDirectoryPath;
    static bool                 exitApp;
};

#endif // CONSOLEHANDLER_H