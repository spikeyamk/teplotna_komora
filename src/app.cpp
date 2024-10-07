#include <optional>
#include <array>
#include <trielo/trielo.hpp>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"
#include "cmsis_os2.h"
#include "submodule/public.hpp"
#include "i2c2/test.hpp"
#include "ds3231/test.hpp"
#include "app.hpp"

extern "C" void Error_Handler();

void init_bsp_com() {
  /* USER CODE BEGIN 2 */
  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  COM_InitTypeDef BspCOMInit;
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }
}

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    UNUSED(arg);
    init_bsp_com();
    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    //Trielo::trielo<returns_true>();
    i2c2::hw::test();
    //i2c2::sw::test();
    //i2c2::stm32_bitbang_i2c::test();
    //ds3231::test();
    for(size_t i = 0; true; i++) {
        std::printf("%u: app_main\n", i);
        osDelay(10'000);
    }
}