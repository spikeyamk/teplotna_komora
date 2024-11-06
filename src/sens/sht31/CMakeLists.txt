add_subdirectory(sht31_driver)
add_library(sens::sht31_driver ALIAS sht31_driver)

add_subdirectory(sht31_regs)
add_library(sens::sht31_regs ALIAS sht31_regs)