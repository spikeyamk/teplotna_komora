add_subdirectory(tasks_common)
add_library(tasks::common ALIAS tasks_common)

add_subdirectory(tasks_example)
add_library(tasks::example ALIAS tasks_example)