add_subdirectory(tasks_common)
add_library(tasks::common ALIAS tasks_common)

add_subdirectory(tasks_panel)
add_library(tasks::panel ALIAS tasks_panel)

add_subdirectory(tasks_rs232_uart)
add_library(tasks::rs232_uart ALIAS tasks_rs232_uart)

add_subdirectory(tasks_senser_killer)
add_library(tasks::senser_killer ALIAS tasks_senser_killer)

add_subdirectory(tasks_sevseg_white)
add_library(tasks::sevseg_white ALIAS tasks_sevseg_white)

add_subdirectory(tasks_temp_ctl)
add_library(tasks::temp_ctl ALIAS tasks_temp_ctl)