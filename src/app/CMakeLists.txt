get_filename_component(SUBDIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE SUBDIRECTORY_SOURCES "*.cpp")
add_library(${SUBDIRECTORY_NAME} OBJECT ${SUBDIRECTORY_SOURCES})
target_include_directories(${SUBDIRECTORY_NAME} PUBLIC "include")
target_link_libraries(${SUBDIRECTORY_NAME} PRIVATE
    stm32cubef2
    trielo
)
target_link_libraries("${SUBDIRECTORY_NAME}" PRIVATE
    actu::bridge
    actu::buzzer
    actu::fan
    actu::lin_source
    actu::pump

    bksram

    comm::usb_uart

    panel::button
    panel::encoder
    panel::led
    panel::green_yellow
    sevseg_common
    panel::white

    peripheral::dac
    peripheral::gpio
    peripheral::it
    peripheral::spi
    peripheral::tim
    peripheral::uart

    sens::i2c_common
    sens::i2c_light
    sens::i2c_temp_hum
    sens::spi_temp

    util
    example_subdirectory
    rtc
)