cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 23)
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)
project(${SUBDIRECTORY_NAME}_dump_uml CXX)

add_subdirectory(${TRIELO_DIR} ${CMAKE_BINARY_DIR}/trielo)
add_subdirectory(${SML_DIR} ${CMAKE_BINARY_DIR}/sml)
add_subdirectory(${PFR_DIR} ${CMAKE_BINARY_DIR}/pfr)
add_subdirectory(${SERDE_DIR} ${CMAKE_BINARY_DIR}/serde)
add_subdirectory(${MAGIC_DIR} ${CMAKE_BINARY_DIR}/magic)

add_executable(${SUBDIRECTORY_NAME}_dump_uml main.cpp)
target_link_libraries(${SUBDIRECTORY_NAME}_dump_uml PUBLIC
    trielo
    sml
    magic
)
target_include_directories(${SUBDIRECTORY_NAME}_dump_uml PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../include
    ${UTIL_DIR}/include
)