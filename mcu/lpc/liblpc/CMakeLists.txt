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

project(liblpc)

include(../hackrf-common.cmake)

set(SRC_M4
		i2c_18xx_43xx.c
		#i2cm_polling.c
		i2c.c
		clock_18xx_43xx.c
		board.c
		chip_18xx_43xx.c
		uart_18xx_43xx.c
		gpio_18xx_43xx.c
		ring_buffer.c
		ssp_18xx_43xx.c
		i2cm_18xx_43xx.c
		wm8904.c
)

include_directories(
		"."
)

DeclareTargets()
