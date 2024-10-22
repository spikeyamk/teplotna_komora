#include <nlohmann/json.hpp>
#include <trielo/trielo.hpp>
#include <ceserial.h>
#include "example_subdirectory/public.hpp"
#include "returns_true.hpp"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int run(int width, int height) {
    (void) width;
    (void) height;
    Trielo::trielo<example_subdirectory::foo>();
    Trielo::trielo<returns_true>();
    const nlohmann::json object {
        { "pi", 3.141 },
        { "happy", true },
        { "name", "Niels" },
        { "nothing", nullptr },
        { "answer",
            {
                {"everything", 42}
            }
        },
        { "list", {1, 0, 2} },
        { "object",
            {
                {"currency", "USD"},
                {"value", 42.99}
            }
        }
    };
    std::cout << std::setw(4) << object << std::endl;


#ifdef CE_WINDOWS
	ceSerial com("\\\\.\\COM4",9600,8,'N',1); // Windows
#else
	ceSerial com("/dev/ttyS0",9600,8,'N',1); // Linux
#endif

	std::printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) {
		std::printf("OK.\n");
	}
	else {
		std::printf("Error.\n");
		return 1;
	}

	bool successFlag;
	std::printf("Writing.\n");
	char s[]="Hello";
	successFlag=com.Write(s); // write string
	successFlag=com.WriteChar('!'); // write a character

	std::printf("Waiting 3 seconds.\n");
	ceSerial::Delay(3000); // delay to wait for a character

	std::printf("Reading.\n");
	char c=com.ReadChar(successFlag); // read a char
	if(successFlag) std::printf("Rx: %c\n",c);

	std::printf("Closing port %s.\n",com.GetPort().c_str());
	com.Close();
    return 0;
}