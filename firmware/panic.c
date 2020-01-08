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
Panic routine.

=============================================================================*/

#include <runtime.h>

#if defined(__VIDEOCORE4__)
	#include <cpu.h>
#else
	#include <hang_cpu.h>
#endif

#include <hang_cpu.h>

#undef panic

void panic(const char* fmt,  ...) {
	printf("panic(): ");

	va_list arp;

	va_start(arp, fmt);
	vprintf(fmt, arp);
	va_end(arp);

	putchar('\n');

	hang_cpu();
}
