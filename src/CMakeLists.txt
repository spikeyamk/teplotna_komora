add_subdirectory("submodule")

add_library("${CMAKE_PROJECT_NAME}_lib"
    "app.cpp"
    "returns_true.cpp"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}_lib" PRIVATE STM32H503xx)
target_link_libraries("${CMAKE_PROJECT_NAME}_lib" PRIVATE submodule)
target_link_libraries("${CMAKE_PROJECT_NAME}_lib" PRIVATE trielo stm32h5xx_hal_driver)
target_include_directories("${CMAKE_PROJECT_NAME}_lib" PUBLIC "core/include")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
add_executable(${CMAKE_PROJECT_NAME}
    "main.c"
    "core/startup_stm32h503rbtx.s"
    "core/syscalls.c"
    "core/sysmem.c"
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_PROJECT_NAME}_lib")
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/src/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Running objcopy to generate BIN file from ELF"
)