add_subdirectory(magic)

if(ENABLE_TESTS)
    set(max31865_regs_TEST TRUE)
endif()
add_subdirectory(max31865_regs)
add_library(sens::max31865_regs ALIAS max31865_regs)

add_subdirectory(sht31_regs)
add_library(sens::sht31_regs ALIAS sht31_regs)