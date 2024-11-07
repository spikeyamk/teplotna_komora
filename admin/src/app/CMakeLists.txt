get_filename_component(SUBDIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB SUBDIRECTORY_SOURCES "*.cpp")

set(SUBDIRECTORY_TEST_VAR "${SUBDIRECTORY_NAME}_TEST")
if(DEFINED ${SUBDIRECTORY_TEST_VAR})
    if(${${SUBDIRECTORY_TEST_VAR}} STREQUAL TRUE)
        ExternalProject_Add(${SUBDIRECTORY_NAME}_test
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test
            CMAKE_GENERATOR Ninja
            CMAKE_CACHE_ARGS
                -DSUBDIRECTORY_NAME:STRING=${SUBDIRECTORY_NAME}
                -DSUBDIRECTORY_SOURCES:STRING=${SUBDIRECTORY_SOURCES}
                -DSUBDIRECTORY_DEPS:STRING=${CMAKE_SOURCE_DIR}/deps/trielo
            CONFIGURE_HANDLED_BY_BUILD FALSE
            BUILD_ALWAYS TRUE
            INSTALL_COMMAND ""
            TEST_COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            TEST_BEFORE_INSTALL TRUE
        )
    endif()
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SUBDIRECTORY_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "include")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    trielo
    nlohmann_json
    serial
    common::magic
)

# Copy usage.txt to the build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/usage.txt ${CMAKE_CURRENT_BINARY_DIR}/usage.txt COPYONLY)

# Install the executable and usage.txt
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/usage.txt DESTINATION ${CMAKE_INSTALL_BINDIR})