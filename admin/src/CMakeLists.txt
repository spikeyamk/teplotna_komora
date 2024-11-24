add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    to_json.cpp
    main_window.cpp
    console.cpp
    control.cpp
    charts.cpp
    transceiver.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    magic
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