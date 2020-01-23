#include <chainloader.h>
#include <hardware.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uintptr_t* _end;
extern uint8_t __bss_start;
extern uint8_t __bss_end;

void main(bool);

#define logf(fmt, ...) { print_timestamp(); printf("[BRINGUP:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__); }

static void heap_init() {
	void* start_of_heap = (void*)MEM_HEAP_START;
	size_t hs = MEM_HEAP_SIZE;

	logf("Initializing heap at 0x%p with size 0x%d\n", start_of_heap, hs);

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

void c_entry(bool security_supported) {
  bzero(&__bss_start, &__bss_end - &__bss_start);
  main(security_supported);
}

void main(bool security_supported) {
	/* wait for peripheral access */
	while(ARM_ID != ARM_IDVAL);
	udelay(30000); // the next 2 prints get cut off if this delay is lower

	logf("Started on ARM, continuing boot from here ...\n");

	logf("Firmware data: SDRAM_SIZE=%lu, VPU_CPUID=0x%lX\n",
	     g_FirmwareData.sdram_size,
	     g_FirmwareData.vpu_cpuid);

        uint32_t arm_cpuid;
        // read MIDR reg
        __asm__("mrc p15, 0, %0, c0, c0, 0" : "=r"(arm_cpuid));
        // from https://github.com/dwelch67/raspberrypi/blob/master/boards/cpuid/cpuid.c
        switch (arm_cpuid) {
        case 0x410FB767:
          logf("rpi 1/0\n");
          break;
        case 0x410FC075:
          logf("rpi 2\n");
          break;
        case 0x410FD034:
          logf("cortex A53, rpi 3\n");
          break;
        // 410FD083 is cortex A72, rpi4
        default:
          logf("unknown rpi model, cpuid is 0x%lx\n", arm_cpuid);
        }

	if (security_supported) {
		logf("Security extensions are supported! but NS bit set\n");
	}

	logf("Execution mode: %s\n", get_execution_mode_name());
        uint32_t cpsr = arm_get_cpsr();
        logf("CPSR: %lx\n", cpsr);

	heap_init();

#if 0
        double foo = 1.1;
        double bar = 2.2;
        double baz = foo * bar;
        printf("%x\n", baz);
#endif

	/* c++ runtime */
	__cxx_init();

	panic("Nothing else to do!");
}
