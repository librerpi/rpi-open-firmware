/*=============================================================================
Copyright (C) 2016-2017 Authors of rpi-open-firmware
All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

FILE DESCRIPTION
VideoCore4 exceptions.

This file in the public release documents all exception names:
	brcm_usrlib\dag\vmcsx\vcfw\rtos\none\rtos_none.c

=============================================================================*/

#pragma once

#define VC4_EXC_ZERO 0
#define VC4_EXC_MISALIGNED 1
#define VC4_EXC_DIVIDE_BY_ZERO 2
#define VC4_EXC_UNDEF 3
#define VC4_EXC_FORBIDDEN 4
#define VC4_EXC_ILLEGAL_MEM 5
#define VC4_EXC_BUS_ERROR 6
#define VC4_EXC_FPE 7
#define VC4_EXC_ISP 8
#define VC4_EXC_DUMMY 9
#define VC4_EXC_ICACHE 10
#define VC4_EXC_VEC_CORE 11
#define VC4_EXC_L2_ALIAS 12
#define VC4_EXC_BKPT 13

#define VC4_EXC_NAMES \
	"Zero", \
	"Misaligned", \
	"Division by zero", \
	"Undefined instruction", \
	"Forbidden instruction", \
	"Illegal memory", \
	"Bus error", \
	"Floating point exception", \
	"ISP", \
	"Dummy", \
	"ICache", \
	"Vector core exception", \
	"Bad L2 alias", \
	"Breakpoint"
