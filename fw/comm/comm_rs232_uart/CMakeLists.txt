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
                -DTRIELO_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/trielo
                -DSML_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/3rdparty/sml
                -DPFR_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/3rdparty/pfr
                -DSERDE_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/serde
                -DMAGIC_DIR:STRING=${CMAKE_SOURCE_DIR}/common/magic
                -DUTIL_DIR:STRING=${CMAKE_SOURCE_DIR}/fw/util
            CONFIGURE_HANDLED_BY_BUILD FALSE
            BUILD_ALWAYS TRUE
            INSTALL_COMMAND ""
            TEST_COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
            TEST_BEFORE_INSTALL TRUE
        )
    endif()
endif()

if(ENABLE_DOCS)
    ExternalProject_Add(${SUBDIRECTORY_NAME}_dump_uml
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dump_uml
        CMAKE_GENERATOR Ninja
        CMAKE_CACHE_ARGS
            -DSUBDIRECTORY_NAME:STRING=${SUBDIRECTORY_NAME}
            -DTRIELO_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/trielo
            -DSML_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/3rdparty/sml
            -DPFR_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/3rdparty/pfr
            -DSERDE_DIR:STRING=${CMAKE_SOURCE_DIR}/deps/serde
            -DMAGIC_DIR:STRING=${CMAKE_SOURCE_DIR}/common/magic
            -DUTIL_DIR:STRING=${CMAKE_SOURCE_DIR}/fw/util
        CONFIGURE_HANDLED_BY_BUILD FALSE
        BUILD_ALWAYS TRUE
        INSTALL_COMMAND ""
        TEST_COMMAND "" 
    )
endif()

add_library(${SUBDIRECTORY_NAME} ${SUBDIRECTORY_SOURCES})
target_include_directories(${SUBDIRECTORY_NAME} PUBLIC "include")
target_link_libraries(${SUBDIRECTORY_NAME} PUBLIC
    sml
    magic
    util
    stm32cubef2
)
