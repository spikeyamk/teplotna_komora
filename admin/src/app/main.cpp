#include "app.hpp"
#include <ceserial.h>

// Setup serial port for windows or linux
#ifdef CE_WINDOWS
	ceSerial com("\\\\.\\COM4",115200,8,'N',1); // Windows
#else
	ceSerial com("/dev/ttyS0",115200,8,'N',1); // Linux
#endif


int main() {
    run();
}