cmake_minimum_required(VERSION 3.12)
project(rf_uart C ASM)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(NRF52_SDK "/home/wieker/Projects/linux/nrf/sdk")
# or
# -DNRF52_SDK=...

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")

set(ARCH_FLAGS "\
        -mabi=aapcs \
        -mcpu=cortex-m4 \
        -mthumb \
")

set(GLOBAL_FLAGS "${ARCH_FLAGS} \
        -DNRF52810_XXAA \
")

set(CMAKE_ASM_FLAGS "${GLOBAL_FLAGS} \
        -x assembler-with-cpp \
")

set(CMAKE_C_FLAGS "${GLOBAL_FLAGS} \
        -O3 \
        -Wall \
        -Werror \
        -fdata-sections \
        -ffunction-sections \
        -fno-builtin \
        -fno-strict-aliasing \
        -fshort-enums \
        -std=c99 \
        -g \
")

include_directories(
        ${NRF52_SDK}/components/toolchain/cmsis/include
        ${NRF52_SDK}/modules/nrfx/mdk

        ${NRF52_SDK}/components/libraries/delay
        ${NRF52_SDK}/modules/nrfx
        ${NRF52_SDK}/modules/nrfx/templates/nRF52810  # nrfx_config.h
        ${NRF52_SDK}/integration/nrfx
        ${NRF52_SDK}/integration/nrfx/legacy
        ${NRF52_SDK}/components/libraries/util
        ${NRF52_SDK}/components/drivers_nrf/nrf_soc_nosd  # nrf_error.h (alternative: components/softdevice/s132/headers)

        ${NRF52_SDK}/modules/nrfx/drivers/include
        ${NRF52_SDK}/modules/nrfx/hal
        ble_config
        ${NRF52_SDK}/components/libraries/log
        ${NRF52_SDK}/components/libraries/experimental_section_vars
        ${NRF52_SDK}/components/libraries/strerror
        ${NRF52_SDK}/components/libraries/log/src
        ${NRF52_SDK}/components/proprietary_rf/esb

        ../common
)

set(CMAKE_EXE_LINKER_FLAGS "${ARCH_FLAGS} \
        --specs=nano.specs \
        -T${PROJECT_SOURCE_DIR}/blinky.ld \
        -Wl,--gc-sections \
        -L${NRF52_SDK}/modules/nrfx/mdk \
        -lc \
        -lnosys \
        -lm \
")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -o <TARGET>")

add_executable(${CMAKE_PROJECT_NAME}
        rf_uart.c
        ${NRF52_SDK}/modules/nrfx/mdk/gcc_startup_nrf52810.S
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52810.c

        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uart.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
        ${NRF52_SDK}/components/libraries/util/app_util_platform.c

        ${NRF52_SDK}/components/proprietary_rf/esb/nrf_esb.c


        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_saadc.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_timer.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_ppi.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_ppi.c

        ../common/local_nrf_uart.c
        ../common/local_nrf_esb.c
        ../common/nrf_local_adc.c )