#include "chainloader.h"
#include "mmu.h"
#include "drivers/mbr_disk.h"
#include "start.h"

#include <hardware.h>
#include <tlsf/tlsf.h>
#include "drivers/block_device.hpp"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>

extern uintptr_t* _end;
extern uint8_t __bss_start;
extern uint8_t __bss_end;

void main(void);
uint32_t arm_lowlevel_setup();

#define logf(fmt, ...) { print_timestamp(); printf("[BRINGUP:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__); }

static void heap_init() {
  void* start_of_heap = (void*)MEM_HEAP_START;
  size_t hs = MEM_HEAP_SIZE;

  *(uint32_t*)(start_of_heap) = 0;

  logf("Initializing heap at 0x%p with size 0x%x\n", start_of_heap, hs);

  init_memory_pool(hs, start_of_heap);
}

static const char* get_execution_mode_name() {
  uint32_t cpsr = arm_get_cpsr() & ARM32_MODE_MASK;

  switch (cpsr) {
  case ARM32_USR:
    return "User";
  case ARM32_FIQ:
    return "FIQ";
  case ARM32_IRQ:
    return "IRQ";
  case ARM32_SVC:
    return "Supervisor";
  case ARM32_MON:
    return "Secure Monitor";
  case ARM32_ABT:
    return "Abort";
  case ARM32_UND:
    return "Undefined Instruction";
  case ARM32_HYP:
    return "Hypervisor";
  case ARM32_SYS:
    return "System";
  default:
    return "Unknown Mode";
  }
}

static int ramSizeInMb() {
  switch (g_FirmwareData.sdram_size) {
  case kRamSize1GB:
    return 1024;
  case kRamSize512MB:
    return 512;
  case kRamSize256MB:
    return 256;
  case kRamSize128MB:
    return 128;
  case kRamSize2GB:
    return 2048;
  case kRamSize4GB:
    return 4096;
  }
}

void c_entry(uint32_t r0) {
  int corenr;

  /* wait for peripheral access */
  while(ARM_ID != ARM_IDVAL);

  corenr = arm_lowlevel_setup();
  switch (corenr) {
  case 0:
    putchar('1');
    break;
  case 1:
    putchar('2');
    break;
  case 2:
    putchar('3');
    break;
  case 3:
    putchar('4');
    break;
  default:
  {
    uint32_t mpidr;
    puts("unknown corenr, halting");
    break;
  }
  }
  if (corenr == 0) {
    bzero(&__bss_start, &__bss_end - &__bss_start);
    udelay(30000); // the first few prints get cut off if this delay is lower
    heap_init();
    printf("r0 is %lx\n", r0);
    init_page_tables(ramSizeInMb());
    main();
  }
}

uint32_t arm_lowlevel_setup() {
  uint32_t midr;
  uint32_t mpidr;
  bool need_timer = false;
  bool unlock_coproc = false;
  bool unlock_l2 = false;
  bool enable_smp = false;
  bool drop_secure = true;
  uint32_t corenr;

  __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 0" : "=r"(midr));
  __asm__ __volatile__("mrc p15, 0, %0, c0, c0, 5" : "=r"(mpidr));
  corenr = mpidr & 0xf;
  switch (midr) {
  case 0x410FB767: // armv6? rpi 0/1
    printf("MIDR: 0x%lx\nMPIDR: 0x%lx\n", midr, mpidr);
    corenr = 0;
    drop_secure = false;
    break;
  case 0x410FC075: // rpi2
    need_timer = true;
    unlock_coproc = true;
    enable_smp = true;
    break;
  case 0x410FD034: // Cortex A53 rpi3
    need_timer = true;
    unlock_coproc = true;
    unlock_l2 = true;
    break;
  }
  if (need_timer) {
    // setup timer freq
    // based loosely on https://github.com/raspberrypi/tools/blob/509f504e8f130ead31b85603016d94d5c854c10c/armstubs/armstub7.S#L130-L135
    __asm__ __volatile__ ("mcr p15, 0, %0, c14, c0, 0": :"r"(19200000));
  }
  if (unlock_coproc) {
    // NSACR = all copros to non-sec
    __asm__ __volatile__ ("mcr p15, 0, %0, c1, c1, 2": :"r"(0x63ff));
  }
  asm_enable_fpu();   // NSACR + CPACR + fpu opcode
  if (enable_smp) {
    asm_set_ACTLR(1<<6);
  }
  if (unlock_l2) {
    //asm_set_ACTLR(1<<6 | 1<<5 | 1<<4 || 1<<1 | 1<<0);
    set_CPUECTLR();
  }
  if (drop_secure) asm_drop_secure();  // SCR
  enable_icache();    // SCTLR
  return corenr;
}

void main() {
  logf("Started on ARM, continuing boot from here ...\n");

  logf("Firmware data: SDRAM_SIZE=%u(%lu), VPU_CPUID=0x%lX\n",
       ramSizeInMb(),
       g_FirmwareData.sdram_size,
       g_FirmwareData.vpu_cpuid);

  // from https://github.com/dwelch67/raspberrypi/blob/master/boards/cpuid/cpuid.c
  switch (arm_get_midr()) {
  case 0x410FB767:
    logf("rpi 1/0\n");
    break;
  case 0x410FC075:
    logf("rpi 2\n"); // cortex-A7?
    break;
  case 0x410FD034:
    logf("cortex A53, rpi 3\n");
    break;
  // 410FD083 is cortex A72, rpi4
  default:
    logf("unknown rpi model, cpuid is 0x%lx\n", arm_get_midr());
  }
  print_cache_tree();

  logf("Execution mode: %s\n", get_execution_mode_name());
  logf("CPSR: %lx\n", arm_get_cpsr());

  sdhost_init();
  init_mbr_disk();

  /* c++ runtime */
  __cxx_init();

  panic("Nothing else to do!");
}
