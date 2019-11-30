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
MBR based disk interface modelling primary partitions as separate disks.

=============================================================================*/

#include <stdint.h>
#include <chainloader.h>

#include "block_device.hpp"

#define logf(fmt, ...) printf("[MBRDISK:%s]: " fmt, __FUNCTION__, ##__VA_ARGS__);

/*****************************************************************************
 * MBR
 *****************************************************************************/

struct MbrPartition {
	uint8_t		part_flag;		/* bootstrap flags */
	uint8_t		part_shd;		/* starting head */
	uint8_t		part_ssect;		/* starting sector */
	uint8_t		part_scyl;		/* starting cylinder */
	uint8_t		part_typ;		/* partition type */
	uint8_t		part_ehd;		/* end head */
	uint8_t		part_esect;		/* end sector */
	uint8_t		part_ecyl;		/* end cylinder */
#if 0
	uint16_t	part_start_lo;		/* absolute starting ... */
	uint16_t	part_start_hi;		/* ... sector number */
	uint16_t	part_size_lo;		/* partition size ... */
	uint16_t	part_size_hi;		/* ... in sectors */
#endif
	uint32_t part_start;
	uint32_t part_size;
} __attribute__((packed));

#define	MBR_SIG		0xAA55

struct Mbr {
	uint16_t	 mbr_code[223];
	MbrPartition mbr_part[4];
	uint16_t	 mbr_sig;
} __attribute__((packed));

static_assert(sizeof(Mbr) >= 512, "What the fuck");

#define MBR_FAT16 0x04
#define MBR_FAT32 0x0B
#define MBR_FAT32_INT13 0x0C
#define MBR_FAT16_INT13 0x0E
#define MBR_LINUX 0x83
#define MBR_NTFS 0x07

static const char* mbr_fs_to_string(int fs) {
	switch (fs) {
	case MBR_FAT32:
		return "FAT32";
	case MBR_FAT32_INT13:
		return "FAT32-INT13";
	case MBR_FAT16_INT13:
		return "FAT16-INT13";
	case MBR_FAT16:
		return "FAT16";
	case MBR_LINUX:
		return "Linux (ext2/ext3)";
	case MBR_NTFS:
		return "NTFS";
	default:
		return "<Unknown>";
	}
}

struct MbrImpl {
	Mbr* mbr;
	BlockDevice* mmc;

	inline bool validate_signature() {
		return reinterpret_cast<uint16_t>(mbr->mbr_sig) == MBR_SIG;
	}

	template <typename T>
	inline bool read_block(uint8_t volume, uint32_t sector, T* dest_buffer) {
		return read_block(volume, sector, reinterpret_cast<uint32_t*>(dest_buffer));
	}

	inline unsigned int get_block_size() {
		return mmc->block_size;
	}

	inline int get_partition_type(uint8_t volume) {
		if (volume > 3)
			return 0;
		return mbr->mbr_part[volume].part_typ;
	}

	bool read_block(uint8_t volume, uint32_t sector, uint32_t* buf) {
		if (volume > 3)
			return false;

		MbrPartition& p = mbr->mbr_part[volume];

		if (p.part_typ == 0)
			return false;

		return mmc->read_block(p.part_start + sector, buf);
	}

	void read_mbr() {
		logf("Reading master boot record ...\n");

		if (!mmc->read_block(0, mbr)) {
			panic("unable to read master boot record from the SD card");
		}

		if (!validate_signature()) {
			panic("invalid master boot record signature (got 0x%x)", mbr->mbr_sig);
		}

		logf("MBR contents:\n");

		for (int i = 0; i < 4; i++) {
			MbrPartition& p = mbr->mbr_part[i];
			printf("    %d: %s at:%ld size:%ld\n", i, mbr_fs_to_string(p.part_typ), p.part_start, p.part_size);
		}
	}

	MbrImpl() {
		mbr = new Mbr;
		mmc = get_sdhost_device();
		if (!mmc) {
			panic("parent block device not initilalized!");
		}
		read_mbr();
		logf("Disk ready!\n");
	}
};

MbrImpl STATIC_FILESYSTEM g_MbrDisk {};

/*****************************************************************************
 * Wrappers for FatFS.
 *****************************************************************************/

#include "fatfs/diskio.h"

bool g_FatFsDiskInitialized = false;

DSTATUS disk_initialize (BYTE pdrv) {
	/* cache disk initialization state */
	if (g_FatFsDiskInitialized) {
		return static_cast<DRESULT>(0);
	}

	BYTE pt = g_MbrDisk.get_partition_type(pdrv);
	switch (pt) {
	case MBR_FAT32_INT13:
	case MBR_FAT16_INT13:
	case MBR_FAT32:
	case MBR_FAT16:
		logf("Mounting FAT partition %d of type 0x%x\n", pdrv, pt);
		g_FatFsDiskInitialized = true;
		return static_cast<DRESULT>(0);
	}
	logf("Disk %d isn't a FAT volume (partition type is 0x%x)!\n", pdrv, pt);
	return STA_NOINIT;
}

DSTATUS disk_status (BYTE pdrv) {
	return disk_initialize(pdrv);
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
	while (count--) {
		g_MbrDisk.read_block(pdrv, sector, buff);
		sector++;
		buff += g_MbrDisk.get_block_size();
	}
	return (DRESULT)0;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff) {
	switch (cmd) {
	case CTRL_SYNC:
		return (DRESULT)0;
	case GET_SECTOR_SIZE:
		*(WORD*)buff = g_MbrDisk.get_block_size();
		return (DRESULT)0;

	case GET_SECTOR_COUNT:
		*(WORD*)buff = 0;
		return (DRESULT)0;

	case GET_BLOCK_SIZE:
		*(WORD*)buff = 1;
		return (DRESULT)0;
	}
	return RES_PARERR;
}
