#include <iostream>
#include "stm32h7xx_hal.h"
#include "i2c2/i2c2.hpp"

extern "C" void Error_Handler(void);

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  std::printf("HAL_I2C_MspInit\n");
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C1235CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{
  std::printf("HAL_I2C_MspDeInit\n");
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PF0     ------> I2C2_SDA
    PF1     ------> I2C2_SCL
    */
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_1);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

namespace i2c2 {
    namespace hw {
        Bus::Bus() {
            std::printf("i2c2::hw::Bus::Bus()\n");
            /* USER CODE BEGIN I2C2_Init 0 */

            /* USER CODE END I2C2_Init 0 */

            /* USER CODE BEGIN I2C2_Init 1 */

            /* USER CODE END I2C2_Init 1 */
            hi2c2.Instance = I2C2;
            hi2c2.Init.Timing = 0x20B0C8FF;
            hi2c2.Init.OwnAddress1 = 0;
            hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
            hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
            hi2c2.Init.OwnAddress2 = 0;
            hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
            hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
            hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
            if (HAL_I2C_Init(&hi2c2) != HAL_OK)
            {
                Error_Handler();
            }

            /** Configure Analogue filter
             */
            if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
            {
                Error_Handler();
            }

            /** Configure Digital filter
             */
            if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
            {
                Error_Handler();
            }
            /* USER CODE BEGIN I2C2_Init 2 */

            /* USER CODE END I2C2_Init 2 */
        }

        Bus::~Bus() {
            if (HAL_I2C_DeInit(&hi2c2) != HAL_OK) {
                Error_Handler();
            }
        }

        void Bus::scan() {
            std::printf("Scanning I2C bus...\n\r");
            // I2C addresses are 7 bits, so they range from 0x08 to 0x77
            for(uint8_t address = 0x08; address < 0xFF; address++) {
                // Attempt to communicate with the device
                if(HAL_I2C_IsDeviceReady(&hi2c2, (address << 1), 1, TIMEOUT) == HAL_OK) {
                    // If the device responds, print its address
                    std::printf("Device found at 0x%02X\n\r", address);
                    const uint8_t ds3231_address { 0x68 };
                    if(address == ds3231_address) {
                        std::printf("address == ds3231_address\n\r");
                    }
                }
            }
            std::printf("I2C scan complete.\n\r");
        }

        HAL_StatusTypeDef Bus::transmit(const uint16_t address, const std::span<uint8_t>& data) {
            return HAL_I2C_Master_Transmit(&hi2c2, address, data.data(), data.size(), TIMEOUT);
        }

        HAL_StatusTypeDef Bus::receive(const uint16_t address, const std::span<uint8_t>& data) {
            return HAL_I2C_Master_Receive(&hi2c2, address, data.data(), data.size(), TIMEOUT);
        }
    }
}