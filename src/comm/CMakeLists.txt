add_subdirectory(rs232_uart)
add_library(comm::rs232_uart ALIAS rs232_uart)
add_subdirectory(usb_uart)
add_library(comm::usb_uart ALIAS usb_uart)