add_subdirectory("submodule")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_executable(${CMAKE_PROJECT_NAME}
    "main.c"
    "core/startup_stm32h503xx.s"
    "core/stm32h5xx_hal_msp.c"
    "core/stm32h5xx_it.c"
    "core/syscalls.c"
    "core/sysmem.c"
    "core/system_stm32h5xx.c"

    "app.cpp"
    "returns_true.cpp"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE STM32H503xx)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE submodule)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE trielo)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE stm32cubeh5)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "core/include")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Running objcopy to generate BIN file from ELF"
)