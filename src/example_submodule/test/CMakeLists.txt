cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 23)
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)
project(${SUBMODULE_NAME}_test CXX)

add_subdirectory(${SUBMODULE_SUBDIRECTORY_DEPS} ${CMAKE_BINARY_DIR}/trielo)
add_library(${SUBMODULE_NAME}_test_lib ${SUBMODULE_SOURCE_FILES})
target_link_libraries(${SUBMODULE_NAME}_test_lib PUBLIC trielo)
target_include_directories(${SUBMODULE_NAME}_test_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include)

add_executable(${SUBMODULE_NAME}_test test.cpp)
target_link_libraries(${SUBMODULE_NAME}_test PUBLIC ${SUBMODULE_NAME}_test_lib)
add_test(NAME ${SUBMODULE_NAME} COMMAND ${SUBMODULE_NAME}_test)