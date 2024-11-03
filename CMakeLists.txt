cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/gcc-arm-none-eabi.cmake")

set(CMAKE_PROJECT_NAME teplotna_komora)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(${CMAKE_PROJECT_NAME} C CXX ASM)
message("Project name: ${CMAKE_PROJECT_NAME}, build type: ${CMAKE_BUILD_TYPE}")

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)

# Add dependencies and subdirectories
enable_testing()

add_subdirectory(common)
add_subdirectory(deps)
add_subdirectory(src)

# Apply stricter warnings for Debug builds
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4)
    endif()
endif()

# Find and enable Doxygen documentation generation
find_package(Doxygen)

if(DOXYGEN_FOUND)
    include("cmake/enable_doxygen.cmake")
    enable_doxygen()
else ()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif ()

# Add unit tests if necessary
# add_executable("${CMAKE_PROJECT_NAME}_tests" test/test_top.cpp) # target_link_libraries("${CMAKE_PROJECT_NAME}_tests" gtest gtest_main)
# target_link_libraries("${CMAKE_PROJECT_NAME}_tests" "${CMAKE_PROJECT_NAME}_lib")
# add_test(NAME "${CMAKE_PROJECT_NAME}_tests" COMMAND "${CMAKE_PROJECT_NAME}_tests")