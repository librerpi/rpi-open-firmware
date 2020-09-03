SRCS = \
	start.s \
	mmu_asm.S \
	lib_armv6/arm_bcopy.s \
	lib_armv6/arm_bzero.s \
	lib_armv6/arm_locore.s \
	lib_armv6/arm_strlen.s \
	lib_armv6/arm_memcmp.s \
	lib_armv6/string_misc.c \
	drivers/uart.c \
	drivers/cprman.cc \
	drivers/libfdt/fdt.c \
	drivers/libfdt/fdt_ro.c \
	drivers/libfdt/fdt_wip.c \
	drivers/libfdt/fdt_rw.c \
	drivers/libfdt/fdt_strerror.c \
	drivers/fatfs/ff.c \
	drivers/sdhost_impl.cc \
	drivers/mbr_disk.cc \
	drivers/mailbox.cc \
	loader.cc \
	trap.cc \
	chainloader.c \
	mmu.c \
	main.c


# rpi1 and up
ARCH = armv6zk

# rpi2 and up
#ARCH = armv7-a

NO_COLOR=""
OK_COLOR=""
ERROR_COLOR=""
WARN_COLOR=""

.PHONY: default all clean create_build_directory device

default: build/arm_chainloader.bin

OBJ := $(addprefix build/, $(addsuffix .o, $(basename $(SRCS))))

CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)gcc
AS = $(CC)
OBJCOPY = $(CROSS_COMPILE)objcopy
LINKFLAGS = -nostdlib -march=$(ARCH) -Wl,--build-id=none -T linker.lds -ltlsf -lgcc -lcommon -lnotc -Wl,--gc-sections -ltlsf
FPUFLAGS=-mfpu=vfp -mfloat-abi=softfp
COMMON_FLAGS = -c -nostdlib -nostartfiles -ffreestanding -march=$(ARCH) -I../ -I./ -I./drivers/libfdt/ -mtune=arm1176jzf-s ${FPUFLAGS} -ffunction-sections -fdata-sections -DBAREMETAL -fno-common

COMMON_FLAGS2 = -g -O3
CFLAGS = $(COMMON_FLAGS) $(COMMON_FLAGS2) -std=c11
CXXFLAGS = $(COMMON_FLAGS) $(COMMON_FLAGS2) -std=c++11 -fno-exceptions -fno-rtti
ASFLAGS = $(COMMON_FLAGS) -x assembler-with-cpp

HEADERS := \
	$(shell find . -type f -name '*.h') \
	$(shell find . -type f -name '*.hpp')

create_build_directory:
	@mkdir -p build

CREATE_SUBDIR = \
	@DIR="$(dir $@)"; \
	if [ ! -d $$DIR ]; then mkdir -p $$DIR; fi

build/start.o: start.s memory_map.h arm-defs.h
build/main.o: main.c chainloader.h mmu.h
build/loader.o: loader.cc chainloader.h memory_map.h mmu.h drivers/fatfs/ff.h drivers/mailbox.hpp drivers/block_device.hpp
build/trap.o: trap.cc
build/chainloader.o: chainloader.c
build/mmu_asm.o: mmu_asm.S
	@echo $(WARN_COLOR)CC  $(NO_COLOR) $@ #-mthumb
	@$(CC) $(CFLAGS) $< -o $@ -march=armv7-a -mtune=cortex-a7

#
# rules to build c/asm files.
#
build/%.o: %.c
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)CC  $(NO_COLOR) $@ #-mthumb
	@$(CC) $(CFLAGS) $< -o $@ #-mthumb

build/%.o: %.cc $(HEADERS)
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)CXX $(NO_COLOR) $@ #-mthumb
	@$(CXX) $(CXXFLAGS) $< -o $@ #-mthumb

build/%.o: %.s
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)AS  $(NO_COLOR) $@
	@$(AS) $(ASFLAGS) $< -o $@

.PRECIOUS: $(OBJ)

build/arm_chainloader.bin: build/arm_chainloader.elf
	@echo $(WARN_COLOR)OBJ$(NO_COLOR) $@
	@$(OBJCOPY) -O binary $< $@

build/arm_chainloader.elf: $(OBJ)
	@echo $(WARN_COLOR)LD  $(NO_COLOR) $@
	@$(CC) $(OBJ) $(LINKFLAGS) -o $@ -Wl,-Map=build/arm_chainloader.map

clean:
	@echo $(ERROR_COLOR)CLEAN$(NO_COLOR)
	@-rm -rf ./build
