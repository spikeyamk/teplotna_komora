add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    nlohmann_json
    common::magic

    Qt6::Core

    sml
    trielo
)