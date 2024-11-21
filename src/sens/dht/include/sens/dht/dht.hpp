/*
 * DHT.h
 *
 *  Created on: Jun 28, 2020
 *      Author: Controllerstech.com
 */

#include <optional>

#include "stm32f2xx_hal.h"

namespace sens {
namespace dht {
	enum class Model {
		DHT11,
		DHT22,
	};

	struct Data {
		float Temperature;
		float Humidity;
	};

	class Extension {
	private:
		const Model model;
		GPIO_TypeDef* port;
		const uint16_t pin;
	public:
		Extension() = delete;
		Extension(const Model model, GPIO_TypeDef* port, const uint16_t pin) :
			model { model },
			port { port },
			pin { pin }
		{}
	private:
		void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
        void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
        void start();
        uint8_t check_response();
        uint8_t read();
	public:
        std::optional<Data> get_data();
	};

	std::optional<Data> get_data();
}
}