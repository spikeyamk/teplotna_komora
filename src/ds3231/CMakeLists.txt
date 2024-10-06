add_library(ds3231 "ds3231.cpp" "test.cpp")
target_include_directories(ds3231 PUBLIC "include")
target_link_libraries(ds3231 PRIVATE stm32cubeh7 i2c2)