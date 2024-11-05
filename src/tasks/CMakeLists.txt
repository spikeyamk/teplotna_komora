add_subdirectory(tasks_common)
add_library(tasks::common ALIAS tasks_common)

add_subdirectory(tasks_rs232_uart)
add_library(tasks::rs232_uart ALIAS tasks_rs232_uart)