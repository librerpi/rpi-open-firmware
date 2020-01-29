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
First stage monitor.

=============================================================================*/

#include <stdio.h>
#include <stdbool.h>

#include <runtime.h>
#include "hardware.h"
#include "arm_monitor.h"

struct tagged_packet {
  uint32_t tag;
  uint32_t value_size;
  uint32_t req_resp;
  uint8_t value[0];
};

bool handle_property_tag(struct tagged_packet *packet);

/*
 * returns false if the tag has been handled
 */
bool handle_property_tag(struct tagged_packet *packet) {
  uint32_t *value32 = (uint32_t*)(&packet->value[0]);
  switch (packet->tag) {
    case 0x3: // firmware hash
      // TODO, truncate the written reply to fit within packet->value_size
      value32[0] = 0x11223344;
      value32[1] = 0x55667788;
      value32[2] = 0x99aabbcc;
      value32[3] = 0xddeeff00;
      value32[4] = 0x11223344;
      packet->req_resp = 0x80000000 | 20;
      return false;
  }
  return true;
}

/*
 * called from sleh_irq (trap.c)
 * other end of the mailbox is in linux/drivers/mailbox/bcm2835-mailbox.c
 * include/soc/bcm2835/raspberrypi-firmware.h and https://github.com/raspberrypi/firmware/wiki/Mailbox-property-interface list tags
 */
void arm_monitor_interrupt() {
  uint32_t msg = ARM_1_MAIL1_RD;
  uint32_t *message = 0;
  printf("VPU MBOX rcv: 0x%lX, cnf 0x%lX\n",
      msg,
      ARM_1_MAIL1_CNF);
  switch (msg & 0xf) {
  case 8: // property tags
    message = (uint32_t*)(msg & ~0xf);
    printf("length: %ld\n", message[0]);
    printf("full message as uint32's\n");
    for (int i=0; i < (message[0]/4); i++) {
      printf("%d: 0x%08lx\n", i, message[i]);
    }
    printf("req/resp: 0x%08lx\n", message[1]);
    bool error = false;
    for (int position = 2; position < (message[0] / 4);) {
      int start = position;
      struct tagged_packet *packet = (struct tagged_packet *)&message[position];
      position += 12; // the header
      position += packet->value_size;
      position += 4 - (packet->value_size % 4); // padding to align
      printf("offset %d, tag 0x%lx, size: %ld, req/resp 0x%lx\n", start, packet->tag, packet->value_size, packet->req_resp);
      error |= handle_property_tag(packet);
    }
    if (error) message[1] = 0x80000001;
    else message[1] = 0x80000000;

    ARM_1_MAIL0_WRT = msg;
    break;
  default:
    printf("unsupported mailbox channel\n");
  }
}

void monitor_start() {
  printf("Starting IPC monitor ...\n");

  /* enable IRQ */
  ARM_1_MAIL1_CNF = ARM_MC_IHAVEDATAIRQEN;

  for(;;) {
    __asm__ __volatile__ ("sleep" :::);
    printf("sleep interrupted!\n");
  }
}
