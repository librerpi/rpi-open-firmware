#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <hexdump.h>
#include <bcm_host.h>

#include <hardware.h>
#include "map_peripherals.h"

#define BV(bit) (1 << bit)

int main_crystal = 54000000; // 54MHz

struct pixel_valve {
  volatile uint32_t c;
  uint32_t vc;
  uint32_t vsyncd_even;
  uint32_t horza;
  uint32_t horzb;
  uint32_t verta;
  uint32_t vertb;
  uint32_t verta_even;
  uint32_t vertb_even;
  uint32_t int_enable;
  uint32_t int_status;
  uint32_t h_active;
};

void dump_pv(void *mmiobase, uint32_t offset, int pvnr) {
  printf("\nPV%d raw dump:\n", pvnr);
  void *pvaddr = reinterpret_cast<void*>(mmiobase) + offset;
  hexdump_ram(pvaddr, 0x7e000000 + offset, 0x80);
  struct pixel_valve pv;
  pixel_valve *rawpv = reinterpret_cast<pixel_valve *>(pvaddr);
  memcpy(&pv, (void*)pvaddr, sizeof(struct pixel_valve));
  int vfp, vbp, vsync, vactive;
  int vfp_even, vbp_even, vsync_even, vactive_even;
  int hfp, hbp, hsync, hactive;

  vfp = (pv.vertb >> 16) & 0xffff;
  vsync = pv.verta & 0xffff;
  vbp = (pv.verta >> 16) & 0xffff;
  vactive = pv.vertb & 0xffff;
  int total_scanlines = vfp + vsync + vbp + vactive;

  vfp_even = (pv.vertb_even >> 16) & 0xffff;
  vsync_even = pv.verta_even & 0xffff;
  vbp_even = (pv.verta_even >> 16) & 0xffff;
  vactive_even = pv.vertb_even & 0xffff;
  int total_scanlines_even = vfp_even + vsync_even + vbp_even + vactive_even;

  hfp = (pv.horzb >> 16) & 0xffff;
  hsync = pv.horza & 0xffff;
  hbp = (pv.horza >> 16) & 0xffff;
  hactive = pv.horzb & 0xffff;
  int scanline_length = hfp + hsync + hbp + hactive;

  if (1) {
    hbp = 30;

    //vsync = 1;
    vactive_even = vactive = 150;
    vfp = 262 - vsync - vbp - vactive;
    vfp_even = 263 - vsync_even - vbp_even - vactive_even;

    //hsync = 1;
    hactive = 720;
    hbp = 60;
    hfp = 858 - hsync - hactive - hbp;
  }

  if (1) {
    rawpv->horza = (hbp << 16) | hsync;
    rawpv->horzb = (hfp << 16) | hactive;

    rawpv->verta = (vbp << 16) | vsync;
    rawpv->vertb = (vfp << 16) | vactive;
    rawpv->verta_even = (vbp_even << 16) | vsync_even;
    rawpv->vertb_even = (vfp_even << 16) | vactive_even;
  }

  if (0) {
    rawpv->c = (pv.c & ~0xc) | (1 << 2);
  }

  printf("C: %x\n", pv.c);
  if (pv.c & BV(0)) puts("  0     enabled");
  if (pv.c & BV(1)) puts("  1     fifo clear");
  printf("  2:3   clock mux channel: %d\n", (pv.c >> 2) & 0x3);
  printf("  4:5   extra clocks per pixel: %d\n", (pv.c >> 4) & 0x3);
  if (pv.c & BV(12)) puts("  12    wait for h-start");
  if (pv.c & BV(13)) puts("  13    trigger underflow");
  if (pv.c & BV(14)) puts("  14    clear at start");
  printf("  15:20 fifo full level: %d\n", (pv.c >> 15) & 0x3f);
  printf("  21:23 format: %d\n", (pv.c >> 21) & 0x7);
  printf("  24:31 unknown: 0x%x\n", pv.c >> 24);
  printf("VC: %x\n", pv.vc);
  if (pv.vc & BV(0)) puts("  video enable");
  if (pv.vc & BV(1)) puts("  contiuous");
  printf("vsyncd_even: %x\n", pv.vsyncd_even);
  if (0) {
    printf("HORZ A: %x B: %x\n", pv.horza, pv.horzb);
    printf("  hsync: %d\n  HBP: %d\n", hsync, hbp);
    printf("  h_active: %d\n  HFP: %d\n", hactive, hfp);
    printf("VERT A: %x B: %x\n", pv.verta, pv.vertb);
    printf("  vsync: %d\n  VBP: %d\n", vsync, vbp);
    printf("  v_active: %d\n  VFP: %d\n", vactive, vfp);
  }
  printf("VERT EVEN A: %x B: %x\n", pv.verta_even, pv.vertb_even);
  printf("INT enable: %x status: %x\n", pv.int_enable, pv.int_status);
  printf("DSI_HACT_ACT: %x\n", pv.h_active);

  puts(  "+-------------------------------------+");
  printf("| front|      |      |        %3d/%3d |\n", vfp, vfp_even);
  printf("|      | sync |      |        %3d/%3d |\n", vsync, vsync_even);
  printf("|      |      | back |        %3d/%3d |\n", vbp, vbp_even);
  printf("| %4d | %4d | %4d | %4d x %3d/%3d |\n", hfp, hsync, hbp, hactive, vactive, vactive_even);
  puts(  "+-------------------------------------+");

  int iDivisor = 0;
  float fDivisior = 0;
  float pixel_clock;
  int input_clock = 0;
  const char *input_name = "";
  switch (pvnr) {
  case 0:
    iDivisor = (CM_DPIDIV >> CM_DPIDIV_DIV_LSB) & CM_DPIDIV_DIV_SET;
    fDivisior = (float)iDivisor / 0x100; // divisor is a 4.8bit int
    int src = CM_DPICTL & 0xf;
    printf("CM_DPI clk src: %d\n", src);
    switch (src) {
    case 1:
      input_clock = main_crystal;
      input_name = "XOSC";
      break;
    }
    if (input_clock > 0) {
      pixel_clock = input_clock / fDivisior;
      printf("pixel clock: %s / %f == %f\n", input_name, fDivisior, pixel_clock/1000/1000);
      printf("hsync clock(%d+%d+%d+%d==%d): %fMHz\n", hfp, hsync, hbp, hactive, scanline_length, pixel_clock / scanline_length /1000/1000);
      printf("vsync clock(%d+%d+%d+%d==%d): %fHz\n", vfp, vsync, vbp, vactive, total_scanlines, pixel_clock / (scanline_length * total_scanlines));
      printf("total clocks per frame: %d\n", scanline_length * total_scanlines);
    }
    break;
  }
}

void print_clock(volatile void *base, uint32_t offset, const char *name) {
  volatile uint32_t *regs = reinterpret_cast<volatile uint32_t*>(base + offset);
  //regs[0] &= ~0x10;
  //regs[1] = 0x5600;
  printf("CM_%sCTL: 0x%x\nCM_%sDIV: 0x%x\n", name, regs[0], name, regs[1]);
  uint32_t s = regs[0];
  int src = s & 0xf;
  int divisor = regs[1] >> 4;
  printf("  src: %d\n  enabled: %d\n  kill: %d\n  busy: %d\n  busyd: %d\n  frac: %d\n", s & 0xf, (s >> 4)&1, (s>>5)&1, (s>>7)&1, (s>>8)&1, (s >> 9)&1);
  printf("  divisor: %f\n", (float)divisor / 0x100);
  switch (src) {
  case 1:
    printf("crystal/(0x%x>>8) == %fMHz\n", divisor, main_crystal / ( ((float)divisor) / 0x100) / 1000 / 1000 );
    break;
  }
}

void dump_hvs(void *mmiobase, int nr, uint32_t listStart, int vc) {
  printf("SCALER_DISPLIST%d: 0x%x\n", nr, listStart);
  //if (listStart == 0) return;
  volatile uint32_t *list = reinterpret_cast<volatile uint32_t*>(mmiobase + 0x402000);
  for (int i=listStart; i<(listStart + 16); i++) {
    printf("%x: 0x%x\n", i, list[i]);
    if (list[i] & (1<<30)) {
      printf("  valid\n");
      printf("  format: %d\n", list[i] & 0xf);
      printf("  pixel order: %d\n", (list[i] >> 13) & 0x3);
      printf("  words: %d\n", (list[i] >> 24) & 0xf);
      if (list[i] & (1<<4)) {
        printf("  unity\n");
        printf("  word0: 0x%x\n", list[i+1]);
        printf("    x: %d y: %d\n", list[i+1] & 0xfff, (list[i+1] >> 12) & 0xfff);
        printf("  word2: 0x%x\n", list[i+2]);
        printf("    width: %d height: %d\n", list[i+2] & 0xffff, (list[i+2] >> 16) & 0xfff);
        printf("  word3: 0x%x\n", list[i+3]);
        printf("  pointer word: 0x%x\n", list[i+4]);
        printf("  pointer context word: 0x%x\n", list[i+5]);
        printf("  pitch word: 0x%x\n", list[i+6]);
        printf("  end?: 0x%x\n", list[i+7]);
        if (list[i+7] & (1<<31)) {
          puts("END");
          break;
        } else {
          i += 6;
        }
      }
    }
  }
}

void print_dpi_state(void *mmiobase) {
  // refer to /drivers/gpu/drm/vc4/vc4_dpi.c
  uint32_t c = DPI_C;
  printf("DPI_C: 0x%x\n", c);
  if (c & BV(0)) puts("  enabled");

  if (c & BV(1)) puts("  output enable disabled");
  if (c & BV(2)) puts("  vsync disabled");
  if (c & BV(3)) puts("  hsync disabled");

  if (c & BV(4)) puts("  output enable negate");
  if (c & BV(5)) puts("  vsync negate");
  if (c & BV(6)) puts("  hsync negate");

  if (c & BV(7)) puts("  output enable invert");
  if (c & BV(8)) puts("  vsync invert");
  if (c & BV(9)) puts("  hsync invert");
  if (c & BV(10)) puts("  pixel clk invert");
}

int main(int argc, char **argv) {
  struct peripherals handle;
  open_peripherals(handle);
  void *mmiobase = handle.peripherals_start;
  //dump_pv(mmiobase, 0x206000, 0);
  //print_clock(rawaddr, 0x101068, "DPI");
  puts("\nVec:");
  print_clock(mmiobase, 0x1010f8, "VEC");
  if (handle.vc == 4) {
    hexdump_ram(mmiobase + 0x806000, 0x7e806000, 0x300);
  } else {
    hexdump_ram(mmiobase + 0xc13000, 0x7ec13000, 0x300);
  }
  //dump_pv(mmiobase, 0x207000, 1);
  if (handle.vc == 4) {
    dump_pv(mmiobase, 0x807000, 2);
  } else if (handle.vc == 6) {
    dump_pv(mmiobase, 0x20a000, 2);
    dump_pv(mmiobase, 0xc12000, 3);
    dump_pv(mmiobase, 0x216000, 4);
  }
  //hexdump_ram(((uint32_t)rawaddr) + 0x200000, 0x7e200000, 0x200);
  //hexdump_ram(mmiobase + 0x400000, 0x7e400000, 0xd0);
  puts("");
  //hexdump_ram(mmiobase + 0x402000, 0x7e402000, 0x100);
  //dump_hvs(mmiobase, 0, SCALER_DISPLIST0, handle.vc);
  dump_hvs(mmiobase, 1, SCALER_DISPLIST1, handle.vc);
  //dump_hvs(mmiobase, 2, SCALER_DISPLIST2, handle.vc);
  //hexdump_ram(mmiobase + 0x402000, 0x7e402000, 0x100);
  //hexdump_ram(mmiobase + 0x404000, 0x7e404000, 0x100);
  //print_dpi_state(mmiobase);
}
