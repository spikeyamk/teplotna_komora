add_subdirectory("actuators")
add_subdirectory("communication")
add_subdirectory("control")
add_subdirectory("display")
add_subdirectory("sensors")
add_subdirectory("tasks")

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

add_executable(${CMAKE_PROJECT_NAME}
    "app.cpp"
    "button_it.cpp"
    "main.c"
    "printf_redirect.c"
)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE STM32F205xx)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    actuators
    communication
    control
    display
    sensors
    tasks
)