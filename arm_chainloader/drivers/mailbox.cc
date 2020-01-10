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
Mailbox driver.

=============================================================================*/

#include "mailbox.hpp"
#include <hardware.h>
#include <stdio.h>

#define logf(fmt, ...) printf("[MBOX:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);

template<typename T>
static bool wait_for_mask(T& reg, uint32_t mask, bool is_set, int timeout) {
	while ((reg & mask) == (is_set ? 0 : mask)) {
		if (timeout == 0)
			return false;
		timeout--;
		udelay(1);
	}

	return true;
}

Bcm2708Mailbox::Bcm2708Mailbox() {

}

bool Bcm2708Mailbox::write_word(uint32_t data, int timeout) {
	if (!wait_for_mask(ARM_0_MAIL1_STA, ARM_MS_FULL, false, timeout)) {
		logf("mailbox write timed out after %dus (STA=0x%lX)\n", timeout, ARM_0_MAIL1_STA);
		return false;
	}

	ARM_0_MAIL1_WRT = data;
	return true;
}

Bcm2708Mailbox STATIC_DRIVER g_Mailbox {};
