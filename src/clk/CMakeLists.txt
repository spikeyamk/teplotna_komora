add_library(clk "clk.cpp")
target_include_directories(clk PUBLIC "include")
target_link_libraries(clk PRIVATE stm32cubeh7)