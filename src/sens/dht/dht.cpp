#include <bitset>

#include "stm32f2xx_hal.h"
#include "util/util.hpp"
#include "sens/dht/dht.hpp"


namespace sens {
namespace dht {
	enum class Model {
		DHT11,
		DHT22,
	};

	static constexpr Model MODEL { Model::DHT11 };

	void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

	void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

	void start() {
		set_pin_output(DHTD_GPIO_Port, DHTD_Pin);  // set the pin as output
		HAL_GPIO_WritePin(DHTD_GPIO_Port, DHTD_Pin, GPIO_PIN_RESET);   // pull the pin low
		if constexpr(MODEL == Model::DHT11) {
			util::microsec_blocking_delay(18000);   // wait for 18ms
		}

		HAL_GPIO_WritePin(DHTD_GPIO_Port, DHTD_Pin, GPIO_PIN_SET);   // pull the pin high
		util::microsec_blocking_delay(20);   // wait for 30us
		set_pin_input(DHTD_GPIO_Port, DHTD_Pin);    // set as input
	}

	uint8_t check_response() {
		uint8_t Response = 0;
		util::microsec_blocking_delay(40);
		if (!(HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin)))
		{
			util::microsec_blocking_delay(80);
			if ((HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin))) Response = 1;
			else Response = -1;
		}
		while ((HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin)));   // wait for the pin to go low

		return Response;
	}

	uint8_t read() {
		uint8_t i,j;
		for (j=0;j<8;j++)
		{
			while (!(HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin)));   // wait for the pin to go high
			util::microsec_blocking_delay(40);   // wait for 40 us
			if (!(HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin)))   // if the pin is low
			{
				i&= ~(1<<(7-j));   // write 0
			}
			else i|= (1<<(7-j));  // if the pin is high, write 1
			while ((HAL_GPIO_ReadPin(DHTD_GPIO_Port, DHTD_Pin)));  // wait for the pin to go low
		}
		return i;
	}

	std::optional<DHT_DataTypedef> get_data() {
		start();
		const uint8_t Presence = check_response();
		const uint8_t Rh_byte1 = read();
		const uint8_t Rh_byte2 = read();
		const uint8_t Temp_byte1 = read();
		const uint8_t Temp_byte2 = read();
		const uint16_t SUM = read();

		if(SUM == (static_cast<uint16_t>(Rh_byte1)+static_cast<uint16_t>(Rh_byte2)+static_cast<uint16_t>(Temp_byte1)+static_cast<uint16_t>(Temp_byte2))) {
			return DHT_DataTypedef {
				.Temperature = static_cast<float>(((std::bitset<16>(Temp_byte1) << 8) | std::bitset<16>(Temp_byte2)).to_ulong()),
				.Humidity = static_cast<float>(((std::bitset<16>(Rh_byte1) << 8) | std::bitset<16>(Rh_byte2)).to_ulong()),
			};
		}

		return std::nullopt;
	}
}
}