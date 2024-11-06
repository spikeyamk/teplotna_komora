add_subdirectory(json)
add_library(serial INTERFACE)
target_include_directories(serial INTERFACE serial)