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
        -DSOFTDEVICE_PRESENT \
        -DS132 \
        -DNRF_SD_BLE_API_VERSION=7 \
        -DNRFX_COREDEP_DELAY_US_LOOP_CYCLES=3 \
        -DNRF52_PAN_74 \
        -DAPP_TIMER_V2_RTC1_ENABLED \
        -DAPP_TIMER_V2 \
        -DFLOAT_ABI_HARD \
")

set(CMAKE_ASM_FLAGS "${GLOBAL_FLAGS} \
        -x assembler-with-cpp \
")

set(CMAKE_C_FLAGS "${GLOBAL_FLAGS} \
        -O3 \
        -Wall \
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

        ${NRF52_SDK}/modules/nrfx/drivers/include
        ${NRF52_SDK}/modules/nrfx/hal
        ble_config
        ${NRF52_SDK}/components/libraries/log
        ${NRF52_SDK}/components/libraries/experimental_section_vars
        ${NRF52_SDK}/components/libraries/strerror
        ${NRF52_SDK}/components/libraries/log/src
        ./

        ${NRF52_SDK}/components/ble/ble_services/ble_ancs_c
        ${NRF52_SDK}/components/ble/ble_services/ble_ias_c
        ${NRF52_SDK}/components/libraries/pwm
        ${NRF52_SDK}/components/softdevice/s112/headers/nrf52
        ${NRF52_SDK}/components/libraries/usbd/class/cdc/acm
        ${NRF52_SDK}/components/libraries/usbd/class/hid/generic
        ${NRF52_SDK}/components/libraries/usbd/class/msc
        ${NRF52_SDK}/components/libraries/usbd/class/hid
        ${NRF52_SDK}/modules/nrfx/hal
        ${NRF52_SDK}/components/libraries/log
        ${NRF52_SDK}/components/ble/ble_services/ble_gls
        ${NRF52_SDK}/components/libraries/fstorage
        ${NRF52_SDK}/components/libraries/mutex
        ${NRF52_SDK}/components/libraries/gpiote
        ${NRF52_SDK}/components/libraries/bootloader/ble_dfu
        ${NRF52_SDK}/components/boards
        ${NRF52_SDK}/components/ble/ble_advertising
        ${NRF52_SDK}/external/utf_converter
        ${NRF52_SDK}/components/ble/ble_services/ble_bas_c
        ${NRF52_SDK}/modules/nrfx/drivers/include
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/
        ${NRF52_SDK}/components/libraries/experimental_task_manager
        ${NRF52_SDK}/components/ble/ble_services/ble_hrs_c
        ${NRF52_SDK}/components/libraries/queue
        ${NRF52_SDK}/components/libraries/pwr_mgmt
        ${NRF52_SDK}/components/ble/ble_dtm
        ${NRF52_SDK}/components/toolchain/cmsis/include
        ${NRF52_SDK}/components/ble/ble_services/ble_rscs_c
        ${NRF52_SDK}/components/ble/common
        ${NRF52_SDK}/components/ble/ble_services/ble_lls
        ${NRF52_SDK}/components/ble/ble_services/ble_bas
        ${NRF52_SDK}/components/libraries/mpu
        ${NRF52_SDK}/components/libraries/experimental_section_vars
        ${NRF52_SDK}/components/ble/ble_services/ble_ans_c
        ${NRF52_SDK}/components/libraries/slip
        ${NRF52_SDK}/components/libraries/delay
        ${NRF52_SDK}/components/libraries/csense_drv
        ${NRF52_SDK}/components/libraries/memobj
        ${NRF52_SDK}/components/ble/ble_services/ble_nus_c
        ${NRF52_SDK}/components/softdevice/common
        ${NRF52_SDK}/components/ble/ble_services/ble_ias
        ${NRF52_SDK}/components/libraries/usbd/class/hid/mouse
        ${NRF52_SDK}/components/libraries/low_power_pwm
        ${NRF52_SDK}/components/ble/ble_services/ble_dfu
        ${NRF52_SDK}/external/fprintf
        ${NRF52_SDK}/components/libraries/svc
        ${NRF52_SDK}/components/libraries/atomic
        ${NRF52_SDK}/components
        ${NRF52_SDK}/components/libraries/scheduler
        ${NRF52_SDK}/components/libraries/cli
        ${NRF52_SDK}/components/ble/ble_services/ble_lbs
        ${NRF52_SDK}/components/ble/ble_services/ble_hts
        ${NRF52_SDK}/components/libraries/crc16
        ${NRF52_SDK}/components/libraries/util
        ${NRF52_SDK}/components/libraries/usbd/class/cdc
        ${NRF52_SDK}/components/libraries/csense
        ${NRF52_SDK}/components/libraries/balloc
        ${NRF52_SDK}/components/libraries/ecc
        ${NRF52_SDK}/components/libraries/hardfault
        ${NRF52_SDK}/components/ble/ble_services/ble_csc
        ${NRF52_SDK}/components/libraries/hci
        ${NRF52_SDK}/components/libraries/timer
        ${NRF52_SDK}/integration/nrfx
        ${NRF52_SDK}/components/libraries/sortlist
        ${NRF52_SDK}/components/libraries/spi_mngr
        ${NRF52_SDK}/components/softdevice/s132/headers
        ${NRF52_SDK}/components/libraries/led_softblink
        ${NRF52_SDK}/components/libraries/sdcard
        ${NRF52_SDK}/modules/nrfx/mdk
        ${NRF52_SDK}/components/ble/ble_services/ble_cts_c
        ${NRF52_SDK}/components/ble/ble_services/ble_nus
        ${NRF52_SDK}/components/libraries/twi_mngr
        ${NRF52_SDK}/components/ble/ble_services/ble_hids
        ${NRF52_SDK}/components/libraries/strerror
        ${NRF52_SDK}/components/libraries/crc32
        ${NRF52_SDK}/components/libraries/usbd/class/audio
        ${NRF52_SDK}/components/ble/peer_manager
        ${NRF52_SDK}/components/libraries/mem_manager
        ${NRF52_SDK}/components/libraries/ringbuf
        ${NRF52_SDK}/components/ble/ble_services/ble_tps
        ${NRF52_SDK}/components/ble/ble_services/ble_dis
        ${NRF52_SDK}/components/ble/nrf_ble_gatt
        ${NRF52_SDK}/components/ble/nrf_ble_qwr
        ${NRF52_SDK}/components/libraries/gfx
        ${NRF52_SDK}/components/libraries/button
        ${NRF52_SDK}/modules/nrfx
        ${NRF52_SDK}/components/libraries/twi_sensor
        ${NRF52_SDK}/integration/nrfx/legacy
        ${NRF52_SDK}/components/libraries/usbd/class/hid/kbd
        ${NRF52_SDK}/external/segger_rtt
        ${NRF52_SDK}/components/libraries/atomic_fifo
        ${NRF52_SDK}/components/ble/ble_services/ble_lbs_c
        ${NRF52_SDK}/components/libraries/crypto
        ${NRF52_SDK}/components/ble/ble_racp
        ${NRF52_SDK}/components/libraries/fds
        ${NRF52_SDK}/components/libraries/atomic_flags
        ${NRF52_SDK}/components/ble/ble_services/ble_hrs
        ${NRF52_SDK}/components/ble/ble_services/ble_rscs
        ${NRF52_SDK}/components/libraries/usbd
        ${NRF52_SDK}/components/libraries/stack_guard
        ${NRF52_SDK}/components/libraries/log/src
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
        main.c
        timer_capture.c
        cross.c
        adc_logic.c
        ble_lbs.c
        gpio_logic.c
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52.c
        ${NRF52_SDK}/modules/nrfx/mdk/gcc_startup_nrf52.S

        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uart.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
        ${NRF52_SDK}/components/libraries/util/app_util_platform.c


        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_rtt.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_serial.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_uart.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_default_backends.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_frontend.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_str_formatter.c
        ${NRF52_SDK}/components/libraries/button/app_button.c
        ${NRF52_SDK}/components/libraries/util/app_error.c
        ${NRF52_SDK}/components/libraries/util/app_error_handler_gcc.c
        ${NRF52_SDK}/components/libraries/util/app_error_weak.c

        ${NRF52_SDK}/external/fprintf/nrf_fprintf.c
        ${NRF52_SDK}/external/fprintf/nrf_fprintf_format.c
        ${NRF52_SDK}/components/libraries/memobj/nrf_memobj.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_uart.c
        ${NRF52_SDK}/modules/nrfx/soc/nrfx_atomic.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uarte.c
        ${NRF52_SDK}/components/libraries/atomic/nrf_atomic.c
        ${NRF52_SDK}/components/libraries/balloc/nrf_balloc.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_ppi.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_ppi.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_timer.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_systick.c
        ${NRF52_SDK}/components/libraries/ringbuf/nrf_ringbuf.c


        ${NRF52_SDK}/components/softdevice/common/nrf_sdh.c
        ${NRF52_SDK}/components/softdevice/common/nrf_sdh_ble.c
        ${NRF52_SDK}/components/softdevice/common/nrf_sdh_soc.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_saadc.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_timer.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_ppi.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_ppi.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT_printf.c
        ${NRF52_SDK}/components/ble/common/ble_advdata.c
        ${NRF52_SDK}/components/ble/common/ble_conn_params.c
        ${NRF52_SDK}/components/ble/common/ble_conn_state.c
        ${NRF52_SDK}/components/ble/common/ble_srv_common.c
        ${NRF52_SDK}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
        ${NRF52_SDK}/components/ble/nrf_ble_qwr/nrf_ble_qwr.c
        ${NRF52_SDK}/external/utf_converter/utf.c
        ${NRF52_SDK}/components/boards/boards.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_clock.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_uart.c
        ${NRF52_SDK}/modules/nrfx/soc/nrfx_atomic.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_clock.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_gpiote.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
        ${NRF52_SDK}/components/libraries/button/app_button.c
        ${NRF52_SDK}/components/libraries/util/app_error.c
        ${NRF52_SDK}/components/libraries/util/app_error_handler_gcc.c
        ${NRF52_SDK}/components/libraries/util/app_error_weak.c
        ${NRF52_SDK}/components/libraries/scheduler/app_scheduler.c
        ${NRF52_SDK}/components/libraries/timer/app_timer2.c
        ${NRF52_SDK}/components/libraries/util/app_util_platform.c
        ${NRF52_SDK}/components/libraries/timer/drv_rtc.c
        ${NRF52_SDK}/components/libraries/hardfault/hardfault_implementation.c
        ${NRF52_SDK}/components/libraries/util/nrf_assert.c
        ${NRF52_SDK}/components/libraries/atomic_fifo/nrf_atfifo.c
        ${NRF52_SDK}/components/libraries/atomic_flags/nrf_atflags.c
        ${NRF52_SDK}/components/libraries/atomic/nrf_atomic.c
        ${NRF52_SDK}/components/libraries/balloc/nrf_balloc.c
        ${NRF52_SDK}/external/fprintf/nrf_fprintf.c
        ${NRF52_SDK}/external/fprintf/nrf_fprintf_format.c
        ${NRF52_SDK}/components/libraries/memobj/nrf_memobj.c
        ${NRF52_SDK}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
        ${NRF52_SDK}/components/libraries/ringbuf/nrf_ringbuf.c
        ${NRF52_SDK}/components/libraries/experimental_section_vars/nrf_section_iter.c
        ${NRF52_SDK}/components/libraries/sortlist/nrf_sortlist.c
        ${NRF52_SDK}/components/libraries/strerror/nrf_strerror.c
)
