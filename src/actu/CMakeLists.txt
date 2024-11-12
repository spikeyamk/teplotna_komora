add_subdirectory(buzzer)
add_library(actu::buzzer ALIAS buzzer)

add_subdirectory(fan)
add_library(actu::fan_common ALIAS fan_common)
add_library(actu::fan_ctl ALIAS fan_ctl)
add_library(actu::fan_fb ALIAS fan_fb)

add_subdirectory(peltier)
add_library(actu::peltier ALIAS peltier)

add_subdirectory(pump)
add_library(actu::pump ALIAS pump)