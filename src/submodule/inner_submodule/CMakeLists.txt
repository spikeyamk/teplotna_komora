add_library(inner_submodule "bar.cpp" "inner_submodule.cpp")
target_include_directories(inner_submodule PUBLIC "include")
target_link_libraries(inner_submodule PRIVATE trielo)