get_filename_component(SUBMODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE SUBMODULE_SOURCE_FILES "*.cpp")
add_library(${SUBMODULE_NAME} OBJECT ${SUBMODULE_SOURCE_FILES})
target_link_libraries(${SUBMODULE_NAME} PRIVATE
    stm32cubef2
    trielo
)
target_link_libraries("${SUBMODULE_NAME}" PRIVATE
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

    example_submodule
)