#add_subdirectory("googletest")

file(GLOB STM32H5XX_HAL_DRIVER_SRCS
    "stm32h5xx_hal_driver/Src/*.c"  # Adjust the path if necessary
)
add_library(stm32h5xx_hal_driver ${STM32H5XX_HAL_DRIVER_SRCS})
target_compile_definitions(stm32h5xx_hal_driver PRIVATE STM32H503xx)
target_include_directories(stm32h5xx_hal_driver PUBLIC "stm32h5xx_hal_driver/Inc")
target_include_directories(stm32h5xx_hal_driver PRIVATE "${CMAKE_SOURCE_DIR}/src/core/include")
target_include_directories(stm32h5xx_hal_driver PRIVATE "${CMAKE_SOURCE_DIR}/deps/3rdparty/cmsis_device_h5/Include")