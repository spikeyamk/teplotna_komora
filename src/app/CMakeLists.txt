get_filename_component(SUBDIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE SUBDIRECTORY_SOURCES "*.cpp")
add_library(${SUBDIRECTORY_NAME} OBJECT ${SUBDIRECTORY_SOURCES})
target_include_directories(${SUBDIRECTORY_NAME} PUBLIC "include")
target_link_libraries(${SUBDIRECTORY_NAME} PRIVATE
    stm32cubef2
    trielo
)
target_link_libraries("${SUBDIRECTORY_NAME}" PRIVATE
    bksram

    comm::usb_uart

    tasks::common
    tasks::rs232_uart

    util
    rtc
)