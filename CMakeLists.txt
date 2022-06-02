cmake_minimum_required(VERSION 3.22)

message("Entering ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")

####################################################################################################
#
# Core project settings
#
####################################################################################################

set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
project(stm32f3-tiny-rtos)
enable_language(C ASM)
message("Build type: "              ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD                23)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)

####################################################################################################
#
# Project's variables
#
####################################################################################################

# Core MCU flags, CPU, instruction set and FPU setup
set(CPU_PARAMETERS
    -mthumb
    -mcpu=cortex-m4
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Symbols definition
set(symbols_c_SYMB
    "USE_HAL_DRIVER"
    "STM32F303xC"
)
set(symbols_asm_SYMB )

# Set linker script and executable
set(linker_script_SRC               ${PROJ_PATH}/STM32F303VCTX_FLASH.ld)
set(EXECUTABLE                      ${CMAKE_PROJECT_NAME})

####################################################################################################
#
# Project's source files and include directories
#
####################################################################################################

# Source files
set(src_core_src_SRCS 
    ${PROJ_PATH}/Core/Src/fifo_queue.c
    ${PROJ_PATH}/Core/Src/main.c
    ${PROJ_PATH}/Core/Src/onboard_user_button.c
    ${PROJ_PATH}/Core/Src/os.c
    ${PROJ_PATH}/Core/Src/os_asm.s
    ${PROJ_PATH}/Core/Src/schedl_timer.c
    ${PROJ_PATH}/Core/Src/stm32f3xx_it.c
    ${PROJ_PATH}/Core/Src/stm32f3xx_hal_msp.c
    ${PROJ_PATH}/Core/Src/syscalls.c
    ${PROJ_PATH}/Core/Src/system_stm32f3xx.c
    ${PROJ_PATH}/Core/Src/sysmem.c
    ${PROJ_PATH}/Core/Src/user_tasks.c)

set(src_core_startup_SRCS 
    ${PROJ_PATH}/Core/Startup/startup_stm32f303vctx.s)

set(src_drivers_stm32f3xx_hal_driver_src_SRCS 
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c)

# Include directories
set(include_c_DIRS
    ${PROJ_PATH}/Core/Inc
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Inc
    ${PROJ_PATH}/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy
    ${PROJ_PATH}/Drivers/CMSIS/Device/ST/STM32F3xx/Include
    ${PROJ_PATH}/Drivers/CMSIS/Include
)
set(include_asm_DIRS )

####################################################################################################
#
# Executable
#
####################################################################################################

# Link directories setup; must be done before executable is added
set(link_DIRS )
link_directories(${EXECUTABLE} ${link_DIRS})

# Executable files
add_executable(${EXECUTABLE} 
    ${src_core_src_SRCS}
    ${src_core_startup_SRCS}
    ${src_drivers_stm32f3xx_hal_driver_src_SRCS})

# Add linked libraries for linker
set(link_LIBS )
target_link_libraries(${EXECUTABLE} ${link_LIBS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE
    # Language specific only
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>: DEBUG>
    $<$<CONFIG:Release>: >
)

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE
    # Language specific only
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>

    # Configuration specific
    $<$<CONFIG:Debug>: >
    $<$<CONFIG:Release>: >
)

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter

    # Language specific only
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:ASM>: -x assembler-with-cpp -MMD -MP>

    # Configuration specific
    $<$<CONFIG:Debug>: -Og -g3 -ggdb>
    $<$<CONFIG:Release>: -Og -g0>
)

# Setup linker parameters
target_link_options(${EXECUTABLE} PRIVATE
    -T${linker_script_SRC}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float              # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage
)

####################################################################################################
#
# Post Build
#
####################################################################################################

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
)

# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
)
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
)

message("Exiting ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
