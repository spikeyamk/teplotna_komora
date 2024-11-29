add_subdirectory(comm_redirect_stdout)
add_library(comm::redirect_stdout ALIAS comm_redirect_stdout)

if(ENABLE_TESTS)
    set(comm_rs232_uart_TEST TRUE)
endif()
add_subdirectory(comm_rs232_uart)
add_library(comm::rs232_uart ALIAS comm_rs232_uart)