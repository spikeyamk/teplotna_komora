add_subdirectory(io_putchar_override)
add_library(comm::io_putchar_override ALIAS io_putchar_override)
add_subdirectory(usb_uart)
add_library(comm::usb_uart ALIAS usb_uart)