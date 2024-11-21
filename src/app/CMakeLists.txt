get_filename_component(SUBDIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE SUBDIRECTORY_SOURCES "*.cpp")
add_library(${SUBDIRECTORY_NAME} OBJECT ${SUBDIRECTORY_SOURCES})
target_include_directories(${SUBDIRECTORY_NAME} PUBLIC "include")
target_link_libraries(${SUBDIRECTORY_NAME} PRIVATE
    stm32cubef2
    trielo
)
target_link_libraries("${SUBDIRECTORY_NAME}" PRIVATE
    actu::buzzer
    actu::fan_ctl
    actu::fan_fb
    actu::pump
    actu::peltier

    bksram

    comm::usb_uart

    panel::button
    panel::encoder
    panel::led
    panel::sevseg_green_yellow
    panel::sevseg_white

    sens::sht31_driver
    sens::max31865_driver

    tasks

    util
)