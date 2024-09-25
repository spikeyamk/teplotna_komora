#add_subdirectory("googletest")

file(GLOB STM32H5XX_HAL_DRIVER_SRCS
    "STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Src/*.c"  # Adjust the path if necessary
)
add_library(stm32h5xx_hal_driver ${STM32H5XX_HAL_DRIVER_SRCS})
target_compile_definitions(stm32h5xx_hal_driver PRIVATE STM32H503xx)
target_compile_definitions(stm32h5xx_hal_driver PRIVATE USE_HAL_DRIVER)
target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/STM32H5xx_HAL_Driver/Inc")
target_include_directories(stm32h5xx_hal_driver PRIVATE "${CMAKE_SOURCE_DIR}/src/core/include")
target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/CMSIS/Device/ST/STM32H5xx/Include")
target_include_directories(stm32h5xx_hal_driver PUBLIC "STM32CubeH5/Drivers/CMSIS/Include")