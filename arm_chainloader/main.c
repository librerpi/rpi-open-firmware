#include <stdint.h>
#include <chainloader.h>
#include <hardware.h>
#include <stdbool.h>

extern uintptr_t* _end;

#define logf(fmt, ...) printf("[BRINGUP:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);

static void heap_init() {
	void* start_of_heap = (void*)MEM_HEAP_START;
	size_t hs = MEM_HEAP_SIZE;

	logf("Initializing heap at 0x%p with size 0x%zu\n", start_of_heap, hs);

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

void main(bool security_supported) {
	/* wait for peripheral access */
	while(ARM_ID != ARM_IDVAL);
	udelay(500);

	logf("Started on ARM, continuing boot from here ...\n");

	logf("Firmware data: SDRAM_SIZE=%lu, VPU_CPUID=0x%lX\n",
	     g_FirmwareData.sdram_size,
	     g_FirmwareData.vpu_cpuid);

	if (security_supported) {
		logf("Security extensions are supported!\n");
	}

	logf("Execution mode: %s\n", get_execution_mode_name());

	heap_init();

	/* c++ runtime */
	__cxx_init();

	panic("Nothing else to do!");
}
