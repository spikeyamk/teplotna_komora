cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(admin CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/ubitint ubitint)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/3rdparty/pfr pfr)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/serde serde)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../deps/3rdparty/sml sml)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../common common)
add_subdirectory(deps)
add_subdirectory(src)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    endif()
endif()

find_package(Doxygen)

if(DOXYGEN_FOUND)
    include("cmake/enable_doxygen.cmake")
    enable_doxygen()
else()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()