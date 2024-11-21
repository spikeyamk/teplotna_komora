#include <bitset>

#include "stm32f2xx_hal.h"
#include "util/util.hpp"
#include "sens/dht/dht.hpp"

namespace sens {
namespace dht {
	static constexpr Model MODEL { Model::DHT11 };

	void Extension::set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
		GPIO_InitTypeDef GPIO_InitStruct {
			.Pin = pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_LOW,
			.Alternate = 0,
		};
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

	void Extension::set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
		GPIO_InitTypeDef GPIO_InitStruct {
			.Pin = pin,
			.Mode = GPIO_MODE_INPUT,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_FREQ_LOW,
			.Alternate = 0,
		};
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

	void Extension::start() {
		set_pin_output(port, pin);  // set the pin as output
		HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);   // pull the pin low
		if constexpr(MODEL == Model::DHT11) {
			util::microsec_blocking_delay(18000);   // wait for 18ms
		}

		HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);   // pull the pin high
		util::microsec_blocking_delay(20);   // wait for 30us
		set_pin_input(port, pin);    // set as input
	}

	uint8_t Extension::check_response() {
		uint8_t Response = 0;
		util::microsec_blocking_delay(40);
		if (!(HAL_GPIO_ReadPin(port, pin)))
		{
			util::microsec_blocking_delay(80);
			if ((HAL_GPIO_ReadPin(port, pin))) Response = 1;
			else Response = -1;
		}
		while ((HAL_GPIO_ReadPin(port, pin)));   // wait for the pin to go low

		return Response;
	}

	uint8_t Extension::read() {
		uint8_t i,j;
		for (j=0;j<8;j++)
		{
			while (!(HAL_GPIO_ReadPin(port, pin)));   // wait for the pin to go high
			util::microsec_blocking_delay(40);   // wait for 40 us
			if (!(HAL_GPIO_ReadPin(port, pin)))   // if the pin is low
			{
				i&= ~(1<<(7-j));   // write 0
			}
			else i|= (1<<(7-j));  // if the pin is high, write 1
			while ((HAL_GPIO_ReadPin(port, pin)));  // wait for the pin to go low
		}
		return i;
	}

	std::optional<Data> Extension::get_data() {
		start();
		const uint8_t Presence = check_response();
		const uint8_t Rh_byte1 = read();
		const uint8_t Rh_byte2 = read();
		const uint8_t Temp_byte1 = read();
		const uint8_t Temp_byte2 = read();
		const uint16_t SUM = read();

		if(SUM == (static_cast<uint16_t>(Rh_byte1)+static_cast<uint16_t>(Rh_byte2)+static_cast<uint16_t>(Temp_byte1)+static_cast<uint16_t>(Temp_byte2))) {
			if(model == Model::DHT11) {
				return Data {
					.Temperature = static_cast<float>((std::bitset<16>(Temp_byte1) << 8).to_ulong()),
					.Humidity = static_cast<float>((std::bitset<16>(Rh_byte1) << 8).to_ulong()),
				};
			} else {
				return Data {
					.Temperature = static_cast<float>(((std::bitset<16>(Temp_byte1) << 8) | std::bitset<16>(Temp_byte2)).to_ulong()),
					.Humidity = static_cast<float>(((std::bitset<16>(Rh_byte1) << 8) | std::bitset<16>(Rh_byte2)).to_ulong()),
				};
			}
		}

		return std::nullopt;
	}
}
}