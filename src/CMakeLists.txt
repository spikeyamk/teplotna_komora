add_subdirectory("actu")
add_subdirectory("comm")
add_subdirectory("panel")
add_subdirectory("sens")
add_subdirectory("submodule")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_executable(${CMAKE_PROJECT_NAME}
    "core/startup_stm32f205xx.s"
    "core/stm32f2xx_hal_msp.c"
    "core/stm32f2xx_it.c"
    "core/syscalls.c"
    "core/sysmem.c"
    "core/system_stm32f2xx.c"

    "main.c"
    "printf_redirect.c"
    "app.cpp"
    "button_it.cpp"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE STM32F205xx)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    actu::bridge
    actu::buzzer
    actu::fan
    actu::lin_source
    actu::pump

    comm::usb_uart

    panel::button
    panel::encoder
    panel::led
    panel::green_yellow
    panel::white

    sens::i2c_common
    sens::i2c_light
    sens::i2c_temp_hum
    #sens::spi_temp

    submodule
)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE trielo)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE stm32cubef2)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "core/include")

# Define the ELF and binary file locations
set(ELF_FILE "${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.elf")
set(BIN_FILE "${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.bin")
set(SILENT_BIN_FILE "${CMAKE_SOURCE_DIR}/misc/silent_firmware.bin")

# Add a custom command to generate the .bin file from the .elf file
add_custom_command(
    OUTPUT ${BIN_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${ELF_FILE} ${BIN_FILE}
    DEPENDS ${ELF_FILE}
    COMMENT "Generating binary file from ELF"
)

# Custom target for flashing
add_custom_target(
    flash
    DEPENDS ${BIN_FILE}  # Ensure the binary is built before flashing
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMAND STM32_Programmer_CLI -c port=SWD -w ${BIN_FILE} 0x08000000 -v -rst
    COMMENT "Building project and flashing the binary with STM32_Programmer_CLI"
)

# Custom target for flashing
add_custom_target(
    flash_silent
    DEPENDS ${SILENT_BIN_FILE}
    COMMAND STM32_Programmer_CLI -c port=SWD -w ${SILENT_BIN_FILE} 0x08000000 -v -rst
    COMMENT "Flashing silent firmware"
)