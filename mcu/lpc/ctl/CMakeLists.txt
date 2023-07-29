# Copyright 2012 Michael Ossmann <mike@ossmann.com>
# Copyright 2012 Jared Boone <jared@sharebrained.com>
#
# This file is part of HackRF.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_TOOLCHAIN_FILE ../toolchain-arm-cortex-m.cmake)

project(blinky)

enable_language(C CXX ASM)

SET(PATH_HACKRF ../..)
SET(PATH_HACKRF_FIRMWARE ${PATH_HACKRF}/firmware)
SET(PATH_HACKRF_FIRMWARE_COMMON ..)
SET(LIBOPENCM3 ..)

	set(MCU_PARTNO LPC4320)

SET(LDSCRIPT_M4_DFU "-T${MCU_PARTNO}_M4_memory.ld -Tlibopencm3_lpc43xx.ld -TLPC43xx_M4_M0_image_from_text.ld")

SET(CFLAGS_COMMON "-Os -g3 -Wall -Wextra ${HACKRF_OPTS} -DDEBUG_ENABLE ${COMMON_FLAGS} -fno-common -MD")
SET(LDFLAGS_COMMON "-nostartfiles -Wl,--gc-sections")

if(V STREQUAL "1")
	SET(LDFLAGS_COMMON "${LDFLAGS_COMMON} -Wl,--print-gc-sections")
endif()

SET(CPUFLAGS_M4 "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
SET(CFLAGS_M4 "-std=gnu99 ${CFLAGS_COMMON} ${CPUFLAGS_M4} -DLPC43XX_M4")
SET(CXXFLAGS_M4 "-std=gnu++0x ${CFLAGS_COMMON} ${CPUFLAGS_M4} -DLPC43XX_M4")
SET(LDFLAGS_M4 "${LDFLAGS_COMMON} ${CPUFLAGS_M4} ${LDSCRIPT_M4} -Xlinker -Map=m4.map")
SET(LDFLAGS_M4_DFU "${LDFLAGS_COMMON} ${CPUFLAGS_M4} ${LDSCRIPT_M4_DFU} -Xlinker -Map=m4.map")

set(BUILD_SHARED_LIBS OFF)

include_directories("${LIBOPENCM3}/include")
include_directories("${LIBOPENCM3}/liblpc")
include_directories("${PATH_HACKRF_FIRMWARE_COMMON}")

macro(DeclareTargets)

	link_directories(
		"${PATH_HACKRF_FIRMWARE_COMMON}"
		"${LIBOPENCM3}"
		"${CMAKE_INSTALL_PREFIX}/lib/armv7e-m/fpu"
	)

	# DFU - using a differnet LD script to run directly from RAM
	add_executable(${PROJECT_NAME}_dfu.elf ${SRC_M4} ../m0_bin.s.obj)

	target_link_libraries(
		${PROJECT_NAME}_dfu.elf
		c
		nosys
		opencm3_lpc43xx
		m
	)

	set_target_properties(${PROJECT_NAME}_dfu.elf PROPERTIES LINK_FLAGS "${LDFLAGS_M4_DFU}")
	set_target_properties(${PROJECT_NAME}_dfu.elf PROPERTIES COMPILE_FLAGS "${CFLAGS_M4}")
endmacro()

set(SRC_M4
	main.c
	gpio_lpc.c
	../liblpc/i2c_18xx_43xx.c
	../liblpc/clock_18xx_43xx.c
	../liblpc/board.c
	../liblpc/chip_18xx_43xx.c
	../liblpc/uart_18xx_43xx.c
	../liblpc/gpio_18xx_43xx.c
	../liblpc/ring_buffer.c
	../liblpc/ssp_18xx_43xx.c
	../liblpc/i2cm_18xx_43xx.c
	../liblpc/wm8904.c
	../liblpc/adc.c
	../liblpc/adc_18xx_43xx.c
	../liblpc/gpdma_18xx_43xx.c
	../liblpc/timers.c
	../liblpc/timer_18xx_43xx.c
	../liblpc/ssp.c
	../liblpc/spi_18xx_43xx.c
	../liblpc/sgpio.c
	../liblpc/uart.c
	../liblpc/sct_18xx_43xx.c
	../liblpc/sct_pwm_18xx_43xx.c
)

DeclareTargets()
