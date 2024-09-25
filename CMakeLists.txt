cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project("cpp-template" C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("deps")
enable_testing()
add_subdirectory("src")

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
else ()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif ()

# Create the test executable
add_executable("${CMAKE_PROJECT_NAME}_tests" test/test_top.cpp)  # Your test file(s)
target_link_libraries("${CMAKE_PROJECT_NAME}_tests" gtest gtest_main)
target_link_libraries("${CMAKE_PROJECT_NAME}_tests" "${CMAKE_PROJECT_NAME}_lib")
add_test(NAME "${CMAKE_PROJECT_NAME}_tests" COMMAND "${CMAKE_PROJECT_NAME}_tests")