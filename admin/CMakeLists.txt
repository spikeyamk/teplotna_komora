cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(admin CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Charts
    Gui
    SerialPort
    Widgets
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/trielo trielo)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/bitint bitint)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/3rdparty/pfr pfr)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/serde serde)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../common common)

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    to_json.cpp
    main_window.cpp
    console.cpp
    control.cpp
    charts.cpp
    transceiver.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    magic
    trielo
    bitint
    sens::max31865_regs
    sens::sht31_regs

    Qt6::Core
    Qt6::Charts
    Qt6::Gui
    Qt6::SerialPort
    Qt6::Widgets
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    endif()
endif()