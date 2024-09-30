add_subdirectory("inner_submodule")
add_library(submodule "private.cpp" "public.cpp")
target_include_directories(submodule PUBLIC "include")
target_link_libraries(submodule PRIVATE trielo inner_submodule)