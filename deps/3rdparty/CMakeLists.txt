add_subdirectory(pfr)
add_subdirectory(sml)

add_library(stm32cubeh7
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_iwdg.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
    STM32CubeH7/Drivers/BSP/STM32H7xx_Nucleo/stm32h7xx_nucleo.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/list.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/queue.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/timers.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

target_compile_definitions(stm32cubeh7 PUBLIC
	USE_HAL_DRIVER
	STM32H723xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubeh7 PUBLIC
    ${CMAKE_SOURCE_DIR}/src/core/include
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Inc
    STM32CubeH7/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/include
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    STM32CubeH7/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    STM32CubeH7/Drivers/BSP/STM32H7xx_Nucleo
    STM32CubeH7/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    STM32CubeH7/Drivers/CMSIS/Include
)

#file(GLOB STM32H5XX_HAL_DRIVER_SRCS
#    "STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/*.c"  # Adjust the path if necessary
#)
#add_library(stm32h5xx_hal_driver ${STM32H5XX_HAL_DRIVER_SRCS})
#target_compile_definitions(stm32h5xx_hal_driver PRIVATE STM32H503xx)
#target_compile_definitions(stm32h5xx_hal_driver PRIVATE USE_HAL_DRIVER)
#target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Inc")
#target_include_directories(stm32h5xx_hal_driver PRIVATE "${CMAKE_SOURCE_DIR}/src/core/include")
#target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/CMSIS/Device/ST/STM32H5xx/Include")
#target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/CMSIS/Include")