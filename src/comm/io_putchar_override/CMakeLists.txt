get_filename_component(SUBMODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
file(GLOB_RECURSE SUBMODULE_SOURCE_FILES "*.cpp")
add_library(${SUBMODULE_NAME} OBJECT ${SUBMODULE_SOURCE_FILES})
target_link_libraries(${SUBMODULE_NAME} PUBLIC stm32cubef2 trielo usb_uart panel::led)