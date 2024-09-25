cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/cubeide-gcc.cmake")

###################### CONSTANTS ######################################
set(PROJECT_TYPE_EXECUTABLE          "exe")
set(PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set(MCPU_CORTEX_M33				     "-mcpu=cortex-m33")
set(MFPU_FPV5_SP_D16                 "-mfpu=fpv5-sp-d16")
set(MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set(RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set(RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")

#######################################################################

###################### VARIABLES ######################################
set(PROJECT_NAME             "blinky_with_cmake")
set(PROJECT_TYPE             "exe")
set(LINKER_SCRIPT            "${CMAKE_CURRENT_LIST_DIR}/misc/STM32H503RBTX_FLASH.ld")
set(MCPU                     "${MCPU_CORTEX_M33}")
set(MFPU                     "${MFPU_FPV5_SP_D16}")
set(MFLOAT_ABI               "${MFLOAT_ABI_HARDWARE}")
set(RUNTIME_LIBRARY          "${RUNTIME_LIBRARY_REDUCED_C}")
set(RUNTIME_LIBRARY_SYSCALLS "${RUNTIME_LIBRARY_SYSCALLS_MINIMAL}")

set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_C_FLAGS "${MCPU} ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb")
set(CMAKE_CXX_FLAGS "${MCPU} ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

# Set C and C++ standards
project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Add dependencies and subdirectories
add_subdirectory("deps")
enable_testing()
add_subdirectory("src")

# Apply stricter warnings for Debug builds
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    endif()
endif()

# Find and enable Doxygen documentation generation
find_package(Doxygen)

if(DOXYGEN_FOUND)
    include("cmake/enable_doxygen.cmake")
    enable_doxygen()
else ()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif ()

# Add unit tests if necessary
# add_executable("${CMAKE_PROJECT_NAME}_tests" test/test_top.cpp)
# target_link_libraries("${CMAKE_PROJECT_NAME}_tests" gtest gtest_main)
# target_link_libraries("${CMAKE_PROJECT_NAME}_tests" "${CMAKE_PROJECT_NAME}_lib")
# add_test(NAME "${CMAKE_PROJECT_NAME}_tests" COMMAND "${CMAKE_PROJECT_NAME}_tests")