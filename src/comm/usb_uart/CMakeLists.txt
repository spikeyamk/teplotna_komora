add_library(usb_uart usb_uart.cpp)
target_include_directories(usb_uart PUBLIC "include")
target_link_libraries(usb_uart stm32cubeh7)