add_subdirectory(pfr)
add_library(sml INTERFACE)
target_include_directories(sml INTERFACE sml)

add_library(stm32cubef2
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dac_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_rcc_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_cortex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_flash_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_pwr_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_gpio.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_dma_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_exti.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_iwdg.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_i2c.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_uart.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/list.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c
)

target_compile_definitions(stm32cubef2 PUBLIC 
	USE_HAL_DRIVER 
	STM32F205xx
	STM32_THREAD_SAFE_STRATEGY=4
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubef2 PUBLIC
    ${CMAKE_SOURCE_DIR}/src/core/include
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Inc
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Inc/Legacy
    STM32CubeF2/Drivers/BSP/STM32F2xx_Nucleo

    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/include
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    STM32CubeF2/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3

    STM32CubeF2/Drivers/CMSIS/Device/ST/STM32F2xx/Include
    STM32CubeF2/Drivers/CMSIS/Include
)