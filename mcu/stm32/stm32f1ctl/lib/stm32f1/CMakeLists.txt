set(STDPERIPH_DIR ${CMAKE_SOURCE_DIR}/lib/stm32f1)
set(CMSIS_DIR ${CMAKE_SOURCE_DIR}/lib/stm32f1/inc)

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${STDPERIPH_DIR}")
  message(FATAL_ERROR "stdperiph submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${CMSIS_DIR}")
  message(FATAL_ERROR "cmsis submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()


# the name of the target operating system
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR ARM)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# core flags
set(CORE_FLAGS "-mthumb -mcpu=cortex-m3 ${ADDITIONAL_CORE_FLAGS}")

# compiler: language specific flags
set(CMAKE_C_FLAGS "${CORE_FLAGS} -fno-builtin -Wall -std=gnu99 -fdata-sections -ffunction-sections -g3 -gdwarf-2" CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS_DEBUG "" CACHE INTERNAL "c compiler flags: Debug")
set(CMAKE_C_FLAGS_RELEASE "" CACHE INTERNAL "c compiler flags: Release")

set(CMAKE_CXX_FLAGS "${CORE_FLAGS} -fno-rtti -fno-exceptions -fno-builtin -Wall -std=gnu++11 -fdata-sections -ffunction-sections -g -ggdb3" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE INTERNAL "cxx compiler flags: Debug")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "cxx compiler flags: Release")

set(CMAKE_ASM_FLAGS "${CORE_FLAGS} -g -ggdb3 -D__USES_CXX" CACHE INTERNAL "asm compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "" CACHE INTERNAL "asm compiler flags: Debug")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "asm compiler flags: Release")

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# find additional toolchain executables
find_program(ARM_SIZE_EXECUTABLE arm-none-eabi-size)
find_program(ARM_GDB_EXECUTABLE arm-none-eabi-gdb)
find_program(ARM_OBJCOPY_EXECUTABLE arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP_EXECUTABLE arm-none-eabi-objdump)


include_directories(
    ${CMSIS_DIR}/core
    ${CMSIS_DIR}/device
    ${STDPERIPH_DIR}/inc
)

set(STDPERIPH_LIB_SRC
    ${STDPERIPH_DIR}/src/misc.c
    ${STDPERIPH_DIR}/src/stm32f10x_adc.c
    ${STDPERIPH_DIR}/src/stm32f10x_bkp.c
    ${STDPERIPH_DIR}/src/stm32f10x_can.c
    ${STDPERIPH_DIR}/src/stm32f10x_cec.c
    ${STDPERIPH_DIR}/src/stm32f10x_crc.c
    ${STDPERIPH_DIR}/src/stm32f10x_dac.c
    ${STDPERIPH_DIR}/src/stm32f10x_dbgmcu.c
    ${STDPERIPH_DIR}/src/stm32f10x_dma.c
    ${STDPERIPH_DIR}/src/stm32f10x_exti.c
    ${STDPERIPH_DIR}/src/stm32f10x_flash.c
    ${STDPERIPH_DIR}/src/stm32f10x_fsmc.c
    ${STDPERIPH_DIR}/src/stm32f10x_gpio.c
    ${STDPERIPH_DIR}/src/stm32f10x_i2c.c
    ${STDPERIPH_DIR}/src/stm32f10x_iwdg.c
    ${STDPERIPH_DIR}/src/stm32f10x_pwr.c
    ${STDPERIPH_DIR}/src/stm32f10x_rcc.c
    ${STDPERIPH_DIR}/src/stm32f10x_rtc.c
    ${STDPERIPH_DIR}/src/stm32f10x_sdio.c
    ${STDPERIPH_DIR}/src/stm32f10x_spi.c
    ${STDPERIPH_DIR}/src/stm32f10x_tim.c
    ${STDPERIPH_DIR}/src/stm32f10x_usart.c
    ${STDPERIPH_DIR}/src/stm32f10x_wwdg.c
)

set(STM32_DEFINES "${STM32_DEFINES} -DUSE_STDPERIPH_DRIVER")

set_source_files_properties(${STDPERIPH_LIB_SRC}
    PROPERTIES COMPILE_FLAGS ${STM32_DEFINES}
)

add_library(stdperiph STATIC ${STDPERIPH_LIB_SRC})

set_target_properties(stdperiph PROPERTIES LINKER_LANGUAGE C)
