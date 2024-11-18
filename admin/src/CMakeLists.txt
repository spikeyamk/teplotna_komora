add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    chart_widget.cpp
    dialog.cpp
    to_json.cpp
    transceiver.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    magic
    sml
    trielo
    bitint
    sens::max31865_regs
    sens::sht31_regs

    Qt6::Core
    Qt6::Charts
    Qt6::Gui
    Qt6::SerialPort
    Qt6::Widgets
)