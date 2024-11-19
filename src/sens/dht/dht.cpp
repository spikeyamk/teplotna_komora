/************** MAKE CHANGES HERE ********************/
#include "stm32f2xx_hal.h"
#include "util/util.hpp"

#include <bitset>

#define TYPE_DHT11    // define according to your sensor
//#define TYPE_DHT22


#define DHT_PORT GPIOG
#define DHT_PIN GPIO_PIN_0




/*******************************************     NO CHANGES AFTER THIS LINE      ****************************************************/

#include "dht/dht.hpp"

void Set_Pin_Output (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT_Start (void)
{
	Set_Pin_Output (DHT_PORT, DHT_PIN);  // set the pin as output
	HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, GPIO_PIN_RESET);   // pull the pin low

#if defined(TYPE_DHT11)
	util::microsec_blocking_delay(18000);   // wait for 18ms
#endif

    HAL_GPIO_WritePin (DHT_PORT, DHT_PIN, GPIO_PIN_SET);   // pull the pin high
    util::microsec_blocking_delay(20);   // wait for 30us
	Set_Pin_Input(DHT_PORT, DHT_PIN);    // set as input
}

uint8_t DHT_Check_Response (void)
{
	uint8_t Response = 0;
	util::microsec_blocking_delay(40);
	if (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))
	{
		util::microsec_blocking_delay(80);
		if ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN))) Response = 1;
		else Response = -1;
	}
	while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));   // wait for the pin to go low

	return Response;
}

uint8_t DHT_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));   // wait for the pin to go high
		util::microsec_blocking_delay(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT_PORT, DHT_PIN)));  // wait for the pin to go low
	}
	return i;
}

std::optional<DHT_DataTypedef> DHT_GetData() {
	DHT_Start ();
	const uint8_t Presence = DHT_Check_Response ();
	const uint8_t Rh_byte1 = DHT_Read ();
	const uint8_t Rh_byte2 = DHT_Read ();
	const uint8_t Temp_byte1 = DHT_Read ();
	const uint8_t Temp_byte2 = DHT_Read ();
	const uint16_t SUM = DHT_Read();

	if (SUM == (static_cast<uint16_t>(Rh_byte1)+static_cast<uint16_t>(Rh_byte2)+static_cast<uint16_t>(Temp_byte1)+static_cast<uint16_t>(Temp_byte2)))
	{
		return DHT_DataTypedef {
			.Temperature = static_cast<float>(((std::bitset<16>(Temp_byte1) << 8) | std::bitset<16>(Temp_byte2)).to_ulong()),
			.Humidity = static_cast<float>(((std::bitset<16>(Rh_byte1) << 8) | std::bitset<16>(Rh_byte2)).to_ulong()),
		};
	}

	return std::nullopt;
}