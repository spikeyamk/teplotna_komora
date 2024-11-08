add_custom_command(
    OUTPUT  ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin
    DEPENDS ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.elf
    COMMENT "Generating binary file from ELF"
)

add_custom_target(
    flash
    DEPENDS ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
    COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -w ${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin 0x08000000 -v -rst
    COMMENT "Building project and flashing the binary with STM32_Programmer_CLI"
)

add_custom_target(
    flash_silent
    DEPENDS ${CMAKE_SOURCE_DIR}/misc/silent_firmware.bin
    COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -w ${CMAKE_SOURCE_DIR}/misc/silent_firmware.bin 0x08000000 -v -rst
    COMMENT "Flashing silent firmware"
)