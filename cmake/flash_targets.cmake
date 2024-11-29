add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/fw/${PELTIA_FW_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_CURRENT_BINARY_DIR}/fw/core/${PELTIA_FW_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/fw/${PELTIA_FW_NAME}.bin
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fw/core/${PELTIA_FW_NAME}.elf
    COMMENT "Generating binary file from ELF"
)

add_custom_target(
    flash
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fw/${PELTIA_FW_NAME}.bin
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target all
    COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -w ${CMAKE_CURRENT_BINARY_DIR}/fw/${PELTIA_FW_NAME}.bin 0x08000000 -v -rst
    COMMENT "Building project and flashing the binary with STM32_Programmer_CLI"
)

add_custom_target(
    flash_silent
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/misc/silent_firmware.bin
    COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -w ${CMAKE_CURRENT_LIST_DIR}/misc/silent_firmware.bin 0x08000000 -v -rst
    COMMENT "Flashing silent firmware"
)