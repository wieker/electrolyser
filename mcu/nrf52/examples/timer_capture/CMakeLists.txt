cmake_minimum_required(VERSION 3.12)
project(timer_capture C ASM)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(NRF52_SDK "/home/wieker/Projects/")
# or
# -DNRF52_SDK=...

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")

set(ARCH_FLAGS "\
        -mabi=aapcs \
        -mcpu=cortex-m4 \
        -mfloat-abi=hard \
        -mfpu=fpv4-sp-d16 \
        -mthumb \
")

set(GLOBAL_FLAGS "${ARCH_FLAGS} \
        -DNRF52 \
        -DNRF52832_XXAA \
        -DBOARD_CUSTOM \
        -DAPP_UART_ENABLED \
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
")

include_directories(
        ${NRF52_SDK}/components/toolchain/cmsis/include
        ${NRF52_SDK}/modules/nrfx/mdk

        ${NRF52_SDK}/components/libraries/delay
        ${NRF52_SDK}/modules/nrfx
        ${NRF52_SDK}/modules/nrfx/templates/nRF52832  # nrfx_config.h
        ${NRF52_SDK}/integration/nrfx
        ${NRF52_SDK}/components/libraries/util
        ${NRF52_SDK}/components/drivers_nrf/nrf_soc_nosd  # nrf_error.h (alternative: components/softdevice/s132/headers)

        ${NRF52_SDK}/modules/nrfx/drivers/include
        ${NRF52_SDK}/modules/nrfx/hal
        ble_config
        ${NRF52_SDK}/components/libraries/log
        ${NRF52_SDK}/components/libraries/experimental_section_vars
        ${NRF52_SDK}/components/libraries/strerror
        ${NRF52_SDK}/components/libraries/log/src
        ./

        ${RF52_SDK}/components/ble/ble_services/ble_ancs_c
        ${RF52_SDK}/components/ble/ble_services/ble_ias_c
        ${RF52_SDK}/components/libraries/pwm
        ${RF52_SDK}/components/softdevice/s112/headers/nrf52
        ${RF52_SDK}/components/libraries/usbd/class/cdc/acm
        ${RF52_SDK}/components/libraries/usbd/class/hid/generic
        ${RF52_SDK}/components/libraries/usbd/class/msc
        ${RF52_SDK}/components/libraries/usbd/class/hid
        ${RF52_SDK}/modules/nrfx/hal
        ${RF52_SDK}/components/libraries/log
        ${RF52_SDK}/components/ble/ble_services/ble_gls
        ${RF52_SDK}/components/libraries/fstorage
        ${RF52_SDK}/components/libraries/mutex
        ${RF52_SDK}/components/libraries/gpiote
        ${RF52_SDK}/components/libraries/bootloader/ble_dfu
        ${RF52_SDK}/components/boards
        ${RF52_SDK}/components/ble/ble_advertising
        ${RF52_SDK}/external/utf_converter
        ${RF52_SDK}/components/ble/ble_services/ble_bas_c
        ${RF52_SDK}/modules/nrfx/drivers/include
        ${RF52_SDK}/modules/nrfx/drivers/src/prs/
        ${RF52_SDK}/components/libraries/experimental_task_manager
        ${RF52_SDK}/components/ble/ble_services/ble_hrs_c
        ${RF52_SDK}/components/libraries/queue
        ${RF52_SDK}/components/libraries/pwr_mgmt
        ${RF52_SDK}/components/ble/ble_dtm
        ${RF52_SDK}/components/toolchain/cmsis/include
        ${RF52_SDK}/components/ble/ble_services/ble_rscs_c
        ${RF52_SDK}/components/ble/common
        ${RF52_SDK}/components/ble/ble_services/ble_lls
        ${RF52_SDK}/components/ble/ble_services/ble_bas
        ${RF52_SDK}/components/libraries/mpu
        ${RF52_SDK}/components/libraries/experimental_section_vars
        ${RF52_SDK}/components/ble/ble_services/ble_ans_c
        ${RF52_SDK}/components/libraries/slip
        ${RF52_SDK}/components/libraries/delay
        ${RF52_SDK}/components/libraries/csense_drv
        ${RF52_SDK}/components/libraries/memobj
        ${RF52_SDK}/components/ble/ble_services/ble_nus_c
        ${RF52_SDK}/components/softdevice/common
        ${RF52_SDK}/components/ble/ble_services/ble_ias
        ${RF52_SDK}/components/libraries/usbd/class/hid/mouse
        ${RF52_SDK}/components/libraries/low_power_pwm
        ${RF52_SDK}/components/ble/ble_services/ble_dfu
        ${RF52_SDK}/external/fprintf
        ${RF52_SDK}/components/libraries/svc
        ${RF52_SDK}/components/libraries/atomic
        ${RF52_SDK}/components
        ${RF52_SDK}/components/libraries/scheduler
        ${RF52_SDK}/components/libraries/cli
        ${RF52_SDK}/components/ble/ble_services/ble_lbs
        ${RF52_SDK}/components/ble/ble_services/ble_hts
        ${RF52_SDK}/components/libraries/crc16
        ${RF52_SDK}/components/libraries/util
        ${RF52_SDK}/components/libraries/usbd/class/cdc
        ${RF52_SDK}/components/libraries/csense
        ${RF52_SDK}/components/libraries/balloc
        ${RF52_SDK}/components/libraries/ecc
        ${RF52_SDK}/components/libraries/hardfault
        ${RF52_SDK}/components/ble/ble_services/ble_csc
        ${RF52_SDK}/components/libraries/hci
        ${RF52_SDK}/components/libraries/timer
        ${RF52_SDK}/integration/nrfx
        ${RF52_SDK}/components/libraries/sortlist
        ${RF52_SDK}/components/libraries/spi_mngr
        ${RF52_SDK}/components/softdevice/s112/headers
        ${RF52_SDK}/components/libraries/led_softblink
        ${RF52_SDK}/components/libraries/sdcard
        ${RF52_SDK}/modules/nrfx/mdk
        ${RF52_SDK}/components/ble/ble_services/ble_cts_c
        ${RF52_SDK}/components/ble/ble_services/ble_nus
        ${RF52_SDK}/components/libraries/twi_mngr
        ${RF52_SDK}/components/ble/ble_services/ble_hids
        ${RF52_SDK}/components/libraries/strerror
        ${RF52_SDK}/components/libraries/crc32
        ${RF52_SDK}/components/libraries/usbd/class/audio
        ${RF52_SDK}/components/ble/peer_manager
        ${RF52_SDK}/components/libraries/mem_manager
        ${RF52_SDK}/components/libraries/ringbuf
        ${RF52_SDK}/components/ble/ble_services/ble_tps
        ${RF52_SDK}/components/ble/ble_services/ble_dis
        ${RF52_SDK}/components/ble/nrf_ble_gatt
        ${RF52_SDK}/components/ble/nrf_ble_qwr
        ${RF52_SDK}/components/libraries/gfx
        ${RF52_SDK}/components/libraries/button
        ${RF52_SDK}/modules/nrfx
        ${RF52_SDK}/components/libraries/twi_sensor
        ${RF52_SDK}/integration/nrfx/legacy
        ${RF52_SDK}/components/libraries/usbd/class/hid/kbd
        ${RF52_SDK}/external/segger_rtt
        ${RF52_SDK}/components/libraries/atomic_fifo
        ${RF52_SDK}/components/ble/ble_services/ble_lbs_c
        ${RF52_SDK}/components/libraries/crypto
        ${RF52_SDK}/components/ble/ble_racp
        ${RF52_SDK}/components/libraries/fds
        ${RF52_SDK}/components/libraries/atomic_flags
        ${RF52_SDK}/components/ble/ble_services/ble_hrs
        ${RF52_SDK}/components/ble/ble_services/ble_rscs
        ${RF52_SDK}/components/libraries/usbd
        ${RF52_SDK}/components/libraries/stack_guard
        ${RF52_SDK}/components/libraries/log/src
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
        timer_capture.c
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52.c

        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uart.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
        ${NRF52_SDK}/components/libraries/util/app_util_platform.c
        )
