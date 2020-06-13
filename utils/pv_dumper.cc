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

#define BV(bit) (1 << bit)

struct pixel_valve {
  uint32_t c;
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

void dump_pv(uint32_t *rawaddr, uint32_t offset, int pvnr) {
  printf("PV%d raw dump:\n", pvnr);
  uint32_t pvaddr = reinterpret_cast<uint32_t>(rawaddr) + offset;
  hexdump_ram(pvaddr, 0x7e000000 + offset, 0x40);
  struct pixel_valve pv;
  volatile pixel_valve *rawpv = reinterpret_cast<volatile pixel_valve *>(pvaddr);
  memcpy(&pv, (void*)pvaddr, sizeof(struct pixel_valve));

  if (0) {
    int vfp, vbp, vsync, vactive;
    vfp = 0;
    vsync = 1;
    vbp = 0;
    vactive = 2;

    int hfp, hbp, hsync, hactive;
    hfp = 0;
    hsync = 1;
    hbp = 0;
    hactive = 10;

    rawpv->horza = (hbp << 16) | hsync;
    rawpv->horzb = (hfp << 16) | hactive;

    rawpv->verta = (vbp << 16) | vsync;
    rawpv->vertb = (vfp << 16) | vactive;
  }

  printf("C: %x\n", pv.c);
  if (pv.c & BV(0)) puts("  enabled");
  if (pv.c & BV(1)) puts("  fifo clear");
  printf("  clock: %d\n", (pv.c >> 2) & 0x3);
  printf("  extra clocks per pixel: %d\n", (pv.c >> 4) & 0x3);
  if (pv.c & BV(12)) puts("  wait for h-start");
  if (pv.c & BV(13)) puts("  trigger underflow");
  if (pv.c & BV(14)) puts("  clear at start");
  printf("  fifo full level: %d\n", (pv.c >> 15) & 0x3f);
  printf("  format: %d\n", (pv.c >> 21) & 0x7);
  printf("VC: %x\n", pv.vc);
  if (pv.vc & BV(0)) puts("  video enable");
  if (pv.vc & BV(1)) puts("  contiuous");
  printf("vsyncd_even: %x\n", pv.vsyncd_even);
  printf("HORZ A: %x B: %x\n", pv.horza, pv.horzb);
  printf("  hsync: %d\n  HBP: %d\n", pv.horza & 0xffff, (pv.horza >> 16) & 0xffff);
  printf("  h_active: %d\n  HFP: %d\n", pv.horzb & 0xffff, (pv.horzb >> 16) & 0xffff);
  printf("VERT A: %x B: %x\n", pv.verta, pv.vertb);
  printf("  vsync: %d\n  VBP: %d\n", pv.verta & 0xffff, (pv.verta >> 16) & 0xffff);
  printf("  v_active: %d\n  VFP: %d\n", pv.vertb & 0xffff, (pv.vertb >> 16) & 0xffff);
  printf("VERT EVEN A: %x B: %x\n", pv.verta_even, pv.vertb_even);
  printf("INT enable: %x status: %x\n", pv.int_enable, pv.int_status);
  printf("DSI_HACT_ACT: %x\n", pv.h_active);
}

int main(int argc, char **argv) {
  int fd = open("/dev/mem", O_RDWR);
  if (fd < 0) {
    perror("unable to open /dev/mem");
    return 2;
  }
  uint32_t arm_phys = bcm_host_get_peripheral_address();
  arm_phys = 0xfe000000;
  printf("arm physical is at 0x%x\n", arm_phys);
  uint32_t *rawaddr = (uint32_t*)mmap(NULL, 16 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, arm_phys);
  if (rawaddr == MAP_FAILED) {
    perror("unable to mmap");
    return 3;
  }
  dump_pv(rawaddr, 0x206000, 0);
  //dump_pv(rawaddr, 0x207000, 1);
  //dump_pv(rawaddr, 0x807000, 2);
  //hexdump_ram(((uint32_t)rawaddr) + 0x200000, 0x7e200000, 0x200);
}
