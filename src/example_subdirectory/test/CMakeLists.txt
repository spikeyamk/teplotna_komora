cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 23)
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)
project(${SUBDIRECTORY_NAME}_test CXX)

add_subdirectory(${SUBDIRECTORY_DEPS} ${CMAKE_BINARY_DIR}/trielo)
add_library(${SUBDIRECTORY_NAME}_test_lib ${SUBDIRECTORY_SOURCES})
target_link_libraries(${SUBDIRECTORY_NAME}_test_lib PUBLIC trielo)
target_include_directories(${SUBDIRECTORY_NAME}_test_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)

add_executable(${SUBDIRECTORY_NAME}_test test.cpp)
target_link_libraries(${SUBDIRECTORY_NAME}_test PUBLIC ${SUBDIRECTORY_NAME}_test_lib)
add_test(NAME ${SUBDIRECTORY_NAME} COMMAND ${SUBDIRECTORY_NAME}_test)