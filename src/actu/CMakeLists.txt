add_subdirectory(bridge)
add_library(actu::bridge ALIAS bridge)

add_subdirectory(buzzer)
add_library(actu::buzzer ALIAS buzzer)

add_subdirectory(fan)
add_library(actu::fan_common ALIAS fan_common)
add_library(actu::fan_ctl ALIAS fan_ctl)
add_library(actu::fan_fb ALIAS fan_fb)
add_library(actu::fan_tim_cb_overrides ALIAS fan_tim_cb_overrides)

add_subdirectory(lin_source)
add_library(actu::lin_source ALIAS lin_source)

add_subdirectory(pump)
add_library(actu::pump ALIAS pump)