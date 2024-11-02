#include <nlohmann/json.hpp>
#include <trielo/trielo.hpp>
#include "decoder/public.hpp"
#include "app.hpp"
#include <ceserial.h>
#include <random>

extern ceSerial com;

int initialize_serial() {
	std::printf("Opening port %s.\n", com.GetPort().c_str());

	if (com.Open() == 0) {
		std::printf("Listening on port %s.\n", com.GetPort().c_str());
	}
	else {
		std::printf("Error opening port %s.\n", com.GetPort().c_str());
		return 1;
	}

	return 0;
}

bool write_to_serial(const std::string& data) {
	std::printf("\n\nSending: %s", data.c_str());

	bool dataSent = com.Write(data.c_str());

	if (dataSent) {
		std::printf("Succesfully sent: %s", data.c_str());
	}
	else {
		std::printf("Failed sending: %s", data.c_str());
	}

	return dataSent;
}

bool write_char_to_serial(const char c) {
	std::printf("\nSending %c ...", c);

	bool dataSent = com.WriteChar(c);

	if (dataSent) {
		std::printf("Succesfully sent: %c \n", c);
	}
	else {
		std::printf("Failed sending: %c!\n", c);
	}

	return dataSent;
}

bool close_serial() {
	std::printf("Closing port %s...\n",com.GetPort().c_str());
	com.Close();
	std::printf("Port %s closed succesfully!\n",com.GetPort().c_str());

	return true;
}

std::string getRandomWord(int length = 6) {
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    std::string word;
    for (int i = 0; i < length; ++i) {
        word += chars[dist(gen)];
    }
    word += '\n';  // Adding \n at the end of the word
    return word;
}

void rceive_word_from_serial(uint8_t size) {
	printf("Received: ");

	for(uint8_t i = 0; i < size; i++) {
		bool successFlag = false;
		com.Delay(100);
		char c = com.ReadChar(successFlag); // read a char

		if(!successFlag || c == '\n') {
			return;
		}else {
			std::printf("%c", c);
		}
	}
}

int run() {
	if(initialize_serial()) {
		return 1;
	}

	while(1) {
		std::string word = getRandomWord();
		write_to_serial(getRandomWord());
		com.Delay(3000);
		rceive_word_from_serial(word.size());
	}

	return 0;
}