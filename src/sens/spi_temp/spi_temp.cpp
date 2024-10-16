#include <iostream>
#include "sens/spi_temp/spi_temp.hpp"
#include "stm32f2xx_hal.h"
#ifdef __cplusplus
extern "C" {
//#include "MAX31865/max31865.h"
}
#endif

extern SPI_HandleTypeDef hspi3;

namespace sens {
namespace spi_temp {
    void chipselect_cb(bool state) {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }

    uint8_t spi_trx_cb(uint8_t data) {
        HAL_SPI_Transmit(&hspi3, &data, 1, HAL_MAX_DELAY);
        uint8_t RX_data { 0 };
        HAL_SPI_Receive(&hspi3, &RX_data, 1, HAL_MAX_DELAY);
        std::printf("spi_trx_cb RX_data: %u\n\r", RX_data);
        return RX_data;
    }

    void charged_time_delay_cb(void) {
        // 10.5*tau delay
    }

    void conversion_timer_delay_cb(void) {
        // 10.5*tau delay
    }

    void threshold_fault(void) {
        // handle threshold fault
    }

    void test() {
        /*
        max31865_t TempSensor;
        max31865_init(
            &TempSensor,
            &chipselect_cb,
            &spi_trx_cb,
            &charged_time_delay_cb,
            &conversion_timer_delay_cb,
            &threshold_fault,
            &threshold_fault,
            0,
            0,
            0,
            0,
            false,
            false
        );
        std::printf("max31865_readADC(&TempSensor): %u\n\r", max31865_readADC(&TempSensor));
        */
    }
}
}