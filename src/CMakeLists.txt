add_subdirectory("actu")
add_subdirectory("app")
add_subdirectory("bksram")
add_subdirectory("comm")
add_subdirectory("core")
add_subdirectory("example_submodule")
add_subdirectory("panel")
add_subdirectory("peripheral")
add_subdirectory("sens")
add_subdirectory("util")

# Define the ELF and binary file locations
set(ELF_FILE "${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.elf")
set(BIN_FILE "${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin")
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