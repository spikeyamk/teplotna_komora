add_subdirectory("submodule")
add_subdirectory("clk")
add_subdirectory("ds3231")
add_subdirectory("i2c2")
add_subdirectory("comm")

file(GLOB_RECURSE CORE_SOURCE_FILES "core/*.s" "core/*.c" "core/*.cpp")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_executable(${CMAKE_PROJECT_NAME}
    ${CORE_SOURCE_FILES}
    "app.cpp"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE STM32H723xx)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    submodule
    ds3231
    i2c2
    usb_uart
)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE trielo)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE stm32cubeh7)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "core/include")

# Define the ELF and binary file locations
set(ELF_FILE "${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.elf")
set(BIN_FILE "${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.bin")
set(FLASH_ADDRESS "0x08000000")

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
    COMMAND STM32_Programmer_CLI -c port=SWD -w ${BIN_FILE} ${FLASH_ADDRESS} -v -rst
    COMMENT "Building project and flashing the binary with STM32_Programmer_CLI"
)