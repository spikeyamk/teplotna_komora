cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/gcc-arm-none-eabi.cmake)

option(ENABLE_TESTS "Enable building of tests" OFF)
if(ENABLE_TESTS)
    include(ExternalProject)
endif()
option(ENABLE_DOCS "Enable building of docs" OFF)
if(ENABLE_DOCS)
    include(ExternalProject)
endif()

set(CMAKE_PROJECT_NAME peltia)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Project name: ${CMAKE_PROJECT_NAME}, build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)

set(PELTIA_FW_NAME peltia_fw)
set(PELTIA_ADMIN_NAME peltia_admin)

add_subdirectory(common)
add_subdirectory(deps)
add_subdirectory(fw)

set(STM32_PROGRAMMER_CLI STM32_Programmer_CLI)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/flash_targets.cmake)

if(ENABLE_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        include("cmake/enable_doxygen.cmake")
        enable_doxygen()
    else()
        message(WARNING "Doxygen not found. Targets 'docs' will not be enabled.")
    endif()
endif()