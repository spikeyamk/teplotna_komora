#add_subdirectory("googletest")

add_library(stm32cubeh5
    STM32CubeH5/Drivers/BSP/STM32H5xx_Nucleo/stm32h5xx_nucleo.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.c
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.c
)

target_compile_definitions(stm32cubeh5 PUBLIC 
	USE_NUCLEO_64 
	USE_HAL_DRIVER 
	STM32H503xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubeh5 PUBLIC
    ${CMAKE_SOURCE_DIR}/src/core/include
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Inc
    STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Inc/Legacy
    STM32CubeH5/Drivers/BSP/STM32H5xx_Nucleo
    STM32CubeH5/Drivers/CMSIS/Device/ST/STM32H5xx/Include
    STM32CubeH5/Drivers/CMSIS/Include
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