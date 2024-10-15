add_subdirectories(${CMAKE_CURRENT_SOURCE_DIR})

# Define the ELF and binary file locations
set(ELF_FILE "${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.elf")
set(BIN_FILE "${CMAKE_BINARY_DIR}/src/core/${CMAKE_PROJECT_NAME}.bin")

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