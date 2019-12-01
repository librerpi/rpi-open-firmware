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
Second stage bootloader.

=============================================================================*/

#include <string.h>
#include <drivers/fatfs/ff.h>
#include <chainloader.h>
#include <drivers/mailbox.hpp>
#include <drivers/block_device.hpp>
#include <libfdt.h>
#include <memory_map.h>

#define logf(fmt, ...) printf("[LDR:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);

FATFS g_BootVolumeFs;

#define ROOT_VOLUME_PREFIX "0:"
#define DTB_LOAD_ADDRESS    0xF000000 // 240mb from start
#define KERNEL_LOAD_ADDRESS 0x2000000 // 32mb from start

typedef void (*linux_t)(uint32_t, uint32_t, void*);

static_assert((MEM_USABLE_START+0x800000) < KERNEL_LOAD_ADDRESS,
              "memory layout would not allow for kernel to be loaded at KERNEL_LOAD_ADDRESS, please check memory_map.h");

struct LoaderImpl {
	inline bool file_exists(const char* path) {
		return f_stat(path, NULL) == FR_OK;
	}

	size_t read_file(const char* path, uint8_t*& dest, bool should_alloc = true) {
		/* ensure file exists first */
		if(!file_exists(path))
			panic("attempted to read %s, but it does not exist", path);

		/* read entire file into buffer */
		FIL fp;
		f_open(&fp, path, FA_READ);

		unsigned int len = f_size(&fp);

		if(should_alloc) {
			/*
			 * since this can be used for strings, there's no harm in reserving an
			 * extra byte for the null terminator and appending it.
			 */
			uint8_t* buffer = new uint8_t[len + 1];
			dest = buffer;
			buffer[len] = 0;
		}

		logf("%s: reading %d bytes to 0x%X (allocated=%d) ...\n", path, len, (unsigned int)dest, should_alloc);

		f_read(&fp, dest, len, &len);
		f_close(&fp);

		return len;
	}

	uint8_t* load_fdt(const char* filename, uint8_t* cmdline) {
		/* read device tree blob */
		uint8_t* fdt = reinterpret_cast<uint8_t*>(DTB_LOAD_ADDRESS);
		size_t sz = read_file(filename, fdt, false);
		logf("FDT loaded at %p, size is %d\n", fdt, sz);

		void* v_fdt = reinterpret_cast<void*>(fdt);

		int res;

		if ((res = fdt_check_header(v_fdt)) != 0) {
			panic("fdt blob invalid, fdt_check_header returned %d", res);
		}

		/* pass in command line args */
		res = fdt_open_into(v_fdt, v_fdt, sz + 4096);

		int node = fdt_path_offset(v_fdt, "/chosen");
		if (node < 0)
			panic("no chosen node in fdt");

		res = fdt_setprop(v_fdt, node, "bootargs", cmdline, strlen((char*) cmdline) + 1);

		/* pass in a memory map, skipping first meg for bootcode */
		int memory = fdt_path_offset(v_fdt, "/memory");
		if(memory < 0)
			panic("no memory node in fdt");

		/* start the memory map at 1M/16 and grow continuous for 256M
		 * TODO: does this disrupt I/O? */

		char dtype[] = "memory";
		uint8_t memmap[] = { 0x00, 0x00, 0x01, 0x00, 0x30, 0x00, 0x00, 0x00 };
		res = fdt_setprop(v_fdt, memory, "reg", (void*) memmap, sizeof(memmap));

		logf("(valid) fdt loaded at 0x%p\n", fdt);

		return fdt;
	}

	void teardown_hardware() {
		BlockDevice* bd = get_sdhost_device();
		if (bd)
			bd->stop();
	}

	LoaderImpl() {
		logf("Mounting boot partitiion ...\n");
		FRESULT r = f_mount(&g_BootVolumeFs, ROOT_VOLUME_PREFIX, 1);
		if (r != FR_OK) {
			panic("failed to mount boot partition, error: %d", (int)r);
		}
		logf("Boot partition mounted!\n");

		/* read the command-line null-terminated */
		uint8_t* cmdline;
		size_t cmdlen = read_file("cmdline.txt", cmdline);

		logf("kernel cmdline: %s\n", cmdline);

		/* load flat device tree */
		uint8_t* fdt = load_fdt("rpi.dtb", cmdline);

		/* once the fdt contains the cmdline, it is not needed */
		delete[] cmdline;

		/* read the kernel as a function pointer at fixed address */
		uint8_t* zImage = reinterpret_cast<uint8_t*>(KERNEL_LOAD_ADDRESS);
		linux_t kernel = reinterpret_cast<linux_t>(zImage);

		size_t ksize = read_file("zImage", zImage, false);
		logf("zImage loaded at 0x%X\n", (unsigned int)kernel);

		/* flush the cache */
		logf("Flushing....\n")
		for (uint8_t* i = zImage; i < zImage + ksize; i += 32) {
			__asm__ __volatile__ ("mcr p15,0,%0,c7,c10,1" : : "r" (i) : "memory");
		}

		/* the eMMC card in particular needs to be reset */
		teardown_hardware();

		/* fire away -- this should never return */
		logf("Jumping to the Linux kernel...\n");
		kernel(0, ~0, fdt);
	}
};

static LoaderImpl STATIC_APP g_Loader {};
