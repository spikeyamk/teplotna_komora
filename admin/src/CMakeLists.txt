add_subdirectory("submodule")

add_library("${CMAKE_PROJECT_NAME}_lib"
    "app.cpp"
    "returns_true.cpp"
)

target_link_libraries("${CMAKE_PROJECT_NAME}_lib" PRIVATE submodule)
target_link_libraries("${CMAKE_PROJECT_NAME}_lib" PRIVATE nlohmann_json trielo)

add_executable(${CMAKE_PROJECT_NAME} "main.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_PROJECT_NAME}_lib")