#ifndef COMMANDSHELPER_H
#define COMMANDSHELPER_H

#include <cstdint>

struct SerializedCommand {
    const char * command;
    uint8_t size;
};

class CommandsHelper {
public:
    SerializedCommand SerializeConnect();
    void DeserializeConnect();
    SerializedCommand SerializeDisconnect();
    void DeserializeDisconnect();
    SerializedCommand SerializeReadSensors(unsigned long ms);
    void DeserializeReadSensors();
    SerializedCommand SerializeWriteTemp();
    void DeserializeWriteTemp();
};

#endif // COMMANDSHELPER_H