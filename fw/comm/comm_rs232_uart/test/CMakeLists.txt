cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 23)
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)
project(${SUBDIRECTORY_NAME}_test CXX)

add_subdirectory(${TRIELO_DIR} ${CMAKE_BINARY_DIR}/trielo)
add_subdirectory(${SML_DIR} ${CMAKE_BINARY_DIR}/sml)
add_subdirectory(${PFR_DIR} ${CMAKE_BINARY_DIR}/pfr)
add_subdirectory(${SERDE_DIR} ${CMAKE_BINARY_DIR}/serde)
add_subdirectory(${MAGIC_DIR} ${CMAKE_BINARY_DIR}/magic)

add_library(${SUBDIRECTORY_NAME}_test_lib test.cpp)
target_link_libraries(${SUBDIRECTORY_NAME}_test_lib PUBLIC
    trielo
    sml
    magic
)
target_include_directories(${SUBDIRECTORY_NAME}_test_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../include
    ${UTIL_DIR}/include
)

file(GLOB SUBDIRECTORY_TEST_CASE_SOURCES "cases/*.cpp")
foreach(SUBDIRECTORY_TEST_CASE_SOURCE ${SUBDIRECTORY_TEST_CASE_SOURCES})
    get_filename_component(SUBDIRECTORY_TEST_CASE_NAME ${SUBDIRECTORY_TEST_CASE_SOURCE} NAME_WE)
    set(SUBDIRECTORY_TEST_TARGET_NAME ${SUBDIRECTORY_NAME}_test_${SUBDIRECTORY_TEST_CASE_NAME})

    add_executable(${SUBDIRECTORY_TEST_TARGET_NAME} ${SUBDIRECTORY_TEST_CASE_SOURCE})
    target_link_libraries(${SUBDIRECTORY_TEST_TARGET_NAME} PUBLIC ${SUBDIRECTORY_NAME}_test_lib)
    add_test(NAME ${SUBDIRECTORY_TEST_TARGET_NAME} COMMAND ${SUBDIRECTORY_TEST_TARGET_NAME})
endforeach()