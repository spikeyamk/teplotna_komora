add_library(i2c2
    "common.cpp" "hw.cpp" "sw.cpp" "test.cpp"
)
target_include_directories(i2c2 PUBLIC "include")
target_link_libraries(i2c2 PRIVATE
    stm32cubeh7
    trielo
    clk
)