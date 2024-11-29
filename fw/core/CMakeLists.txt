set(CMAKE_EXECUTABLE_SUFFIX ".elf")
file(GLOB_RECURSE SUBDIRECTORY_SOURCES "*.s" "*.c" "*.cpp")
add_executable(${PELTIA_FW_NAME} ${SUBDIRECTORY_SOURCES})
target_compile_definitions(${PELTIA_FW_NAME} PRIVATE STM32F205xx)
target_include_directories(${PELTIA_FW_NAME} PUBLIC "include")
target_link_libraries(${PELTIA_FW_NAME} PRIVATE stm32cubef2)
target_link_libraries(${PELTIA_FW_NAME} PRIVATE
    app
    util
    callback_overrides
)