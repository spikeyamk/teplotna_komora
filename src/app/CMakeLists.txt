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
    actu::fan_ctl
    actu::fan_fb
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

    sens::i2c_common
    sens::i2c_light
    sens::i2c_temp_hum
    sens::max31865_driver

    tasks::common
    tasks::panel
    tasks::rs232_uart
    tasks::senser_killer
    tasks::temp_ctl

    util
    rtc
)