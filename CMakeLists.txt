cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/gcc-arm-none-eabi.cmake)

option(ENABLE_TESTS "Enable building of tests" OFF)
if(ENABLE_TESTS)
    include(ExternalProject)
endif()

set(CMAKE_PROJECT_NAME teplotna_komora)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Project name: ${CMAKE_PROJECT_NAME}, build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory("common")
add_subdirectory("deps")
add_subdirectory("src")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/flash_targets.cmake)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    include("cmake/enable_doxygen.cmake")
    enable_doxygen()
else()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()