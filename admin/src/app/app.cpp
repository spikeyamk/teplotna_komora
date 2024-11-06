#include <nlohmann/json.hpp>
#include <trielo/trielo.hpp>
#include "decoder/public.hpp"
#include "app.hpp"
#include <random>
#include "SerialHandler.hpp"
#include "ConsoleHandler.hpp"

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

int run() {
	// SerialHandler serial = SerialHandler();
	ConsoleHandler app = ConsoleHandler();

	// if(!serial.Initialize()) {
	// 	return 1;
	// }

	app.Run();

	// while(1) {
		// std::string word = getRandomWord();
		// serial.SendMessage(word);
		// serial.Delay(3000);
		// serial.ReceiveMessage(word.size());
	//}

	return 0;
}