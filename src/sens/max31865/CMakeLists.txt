add_subdirectory(max31865_driver)
add_library(sens::max31865_driver ALIAS max31865_driver)

if(ENABLE_TESTS)
    set(max31865_regs_TEST TRUE)
endif()
add_subdirectory(max31865_regs)
add_library(sens::max31865_regs ALIAS max31865_regs)