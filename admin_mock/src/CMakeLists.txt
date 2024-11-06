add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    chart_widget.cpp
    dialog.cpp
    to_json.cpp
    transceiver.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    nlohmann_json
    common::magic

    Qt6::Core
    Qt6::Charts
    Qt6::Gui
    Qt6::SerialPort
    Qt6::Widgets

    sml
    trielo
)