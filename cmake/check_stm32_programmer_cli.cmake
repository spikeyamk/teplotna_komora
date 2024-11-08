if(NOT DEFINED STM32_PROGRAMMER_CLI)
    message(FATAL_ERROR "STM32_PROGRAMMER_CLI must be set to the path of STM32_Programmer_CLI.")
endif()
find_program(STM32_PROGRAMMER_EXISTS NAMES ${STM32_PROGRAMMER_CLI})
if(NOT STM32_PROGRAMMER_EXISTS)
    message(FATAL_ERROR "STM32_PROGRAMMER_CLI does not point to a valid executable. Please check the path and try again.")
else()
    message(STATUS "STM32_Programmer_CLI found at: ${STM32_PROGRAMMER_EXISTS}")
endif()