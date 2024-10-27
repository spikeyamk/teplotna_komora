#include <nlohmann/json.hpp>
#include <trielo/trielo.hpp>
#include "example_subdirectory/public.hpp"
#include "returns_true.hpp"
#include "app.hpp"
#include <ceserial.h>

extern ceSerial com;

bool write_to_serial(const std::string& data) {
	std::printf("Sending %s ...\n", com.GetPort().c_str());

	bool dataSent = com.Write(data.c_str());

	if (dataSent) {
		std::printf("Data %s sent succesfully!\n", data.c_str());
	}
	else {
		std::printf("Error sending data %s!\n", data.c_str());
	}

	return dataSent;
}

bool write_char_to_serial(const char c) {
	std::printf("Sending %c ...\n", c);

	bool dataSent = com.WriteChar(c);

	if (dataSent) {
		std::printf("Data %c sent succesfully!\n", c);
	}
	else {
		std::printf("Error sending data %c!\n", c);
	}

	return dataSent;
}

bool close_serial() {
	std::printf("Closing port %s...\n",com.GetPort().c_str());
	com.Close();
	std::printf("Port %s closed succesfully!\n",com.GetPort().c_str());

	return true;
}

int run() {
	std::printf("Opening port %s.\n", com.GetPort().c_str());

	if (com.Open() == 0) {
		std::printf("Listening on port %s.\n", com.GetPort().c_str());
	}
	else {
		std::printf("Error opening port %s.\n", com.GetPort().c_str());
		return 1;
	}

	bool successFlag = false;

	while(1) {
		char c = com.ReadChar(successFlag); // read a char
		if (successFlag) {
			std::printf("%c\n", c);
		} else {
			std::printf("Error reading char!\n");
			close_serial();
			break;
		}
	}

    return 0;
}