#include "peripheral/dac/dac.hpp"

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(dacHandle->Instance==DAC) {
        __HAL_RCC_DAC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        using namespace peripheral::dac;
        GPIO_InitStruct.Pin =
            static_cast<uint32_t>(Pins::LINSA)
            | static_cast<uint32_t>(Pins::LINSB);
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), &GPIO_InitStruct);
    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle) {
    if(dacHandle->Instance==DAC) {
        __HAL_RCC_DAC_CLK_DISABLE();
        using namespace peripheral::dac;
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint32_t>(Pins::LINSA)
            | static_cast<uint32_t>(Pins::LINSB)
        );
    }
}