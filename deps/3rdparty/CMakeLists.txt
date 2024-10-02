#add_subdirectory("googletest")

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
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_i2c.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_spi.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_tim_ex.c
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Src/stm32f2xx_hal_uart.c
)

target_compile_definitions(stm32cubef2 PUBLIC 
	USE_NUCLEO_64 
	USE_HAL_DRIVER 
	STM32F205xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubef2 PUBLIC
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Inc
    STM32CubeF2/Drivers/STM32F2xx_HAL_Driver/Inc/Legacy
    STM32CubeF2/Drivers/BSP/STM32F2xx_Nucleo
    STM32CubeF2/Drivers/CMSIS/Device/ST/STM32F2xx/Include
    STM32CubeF2/Drivers/CMSIS/Include
)

target_link_libraries(stm32cubef2 PUBLIC core)

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