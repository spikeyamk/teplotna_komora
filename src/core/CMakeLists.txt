get_filename_component(SUBMODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_library(${SUBMODULE_NAME}
    startup_stm32f205xx.s
    stm32f2xx_hal_msp.c
    stm32f2xx_it.c
    system_stm32f2xx.c
)
target_include_directories(${SUBMODULE_NAME} PUBLIC "include")
target_link_libraries(${SUBMODULE_NAME} PRIVATE stm32cubef2)