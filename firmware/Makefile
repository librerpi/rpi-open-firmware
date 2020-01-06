#
# when building bootcode.bin, always ensure start.s is at the top, providing
# the 0x200 byte long header and some init code.
#
SRCS = \
	start.s \
	romstage.c \
	sdram.c \
	arm_monitor.cc \
	trap.c \
	lib/memcpy.c \
	drivers/IODevice.cc \
	drivers/BCM2708PowerManagement.cc \
	drivers/BCM2708UsbPhy.cc \
	drivers/BCM2708ArmControl.cc \
	drivers/BCM2708ClockDomains.cc \
	drivers/BCM2708Gpio.cc \
	drivers/gpclk.cc \
	hang_cpu.o \
	utils.cc \
	BCM2708PlatformStartup.cc

ARCH = vc4

BUILD_DIR = build
TARGET_BUILD_DIR = $(BUILD_DIR)/$(ARCH)-objects
PRODUCT_DIRECTORY = $(BUILD_DIR)

NO_COLOR=""
OK_COLOR=""
ERROR_COLOR=""
WARN_COLOR=""

.PHONY: default all clean create_build_directory device

default: bootcode.bin

EXTRA_OBJ := arm_chainloader.o
OBJ := $(addprefix $(TARGET_BUILD_DIR)/, $(addsuffix .o, $(basename $(SRCS)))) $(EXTRA_OBJ)

# the cross compiler should already be in your path
CROSS_COMPILE ?= vc4-elf-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CC)
OBJCOPY = $(CROSS_COMPILE)objcopy
LINKFLAGS = -nostdlib -nostartfiles --build-id=none -T linker.ld --no-omagic # --print-map #-Wl,--gc-sections --entry=_start -Wl,--cref

CFLAGS = -c -nostdlib -Wno-multichar -std=c11 -fsingle-precision-constant -Wdouble-promotion -D__VIDEOCORE4__ -I./vc4_include/ -I./
ASFLAGS = -c -nostdlib -x assembler-with-cpp -D__VIDEOCORE4__ -I./vc4_include/ -I./
CXXFLAGS = -c -nostdlib -Wno-multichar -std=c++11 -fno-exceptions -fno-rtti -D__VIDEOCORE4__ -I./vc4_include/ -I./

HEADERS := \
	$(shell find . -type f -name '*.h') \
	$(shell find . -type f -name '*.hpp')

create_build_directory:
	@mkdir -p $(TARGET_BUILD_DIR)
	@mkdir -p $(PRODUCT_DIRECTORY)

CREATE_SUBDIR = \
	@DIR="$(dir $@)"; \
	if [ ! -d $$DIR ]; then mkdir -p $$DIR; fi


#
# rules to build c/asm files.
#
$(TARGET_BUILD_DIR)/%.o: %.c $(HEADERS)
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)CC  $(NO_COLOR) $@
	@$(CC) $(CFLAGS) $< -o $@

$(TARGET_BUILD_DIR)/%.o: %.cc $(HEADERS)
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)CXX $(NO_COLOR) $@
	@$(CXX) $(CXXFLAGS) $< -o $@

$(TARGET_BUILD_DIR)/%.o: %.s $(HEADERS)
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)AS  $(NO_COLOR) $@
	@$(AS) $(ASFLAGS) $< -o $@

arm_chainloader.o: arm_chainloader/build/arm_chainloader.bin
	$(OBJCOPY) -I binary -O elf32-vc4 -B vc4 $< $@

.PRECIOUS: $(OBJ)

$(PRODUCT_DIRECTORY)/bootcode.elf: create_build_directory $(OBJ)
	@echo $(WARN_COLOR)LD  $(NO_COLOR) $@
	@$(LD) $(LINKFLAGS) $(OBJ) -o $@ -lcommon

bootcode.stub:
	truncate bootcode.stub -s 512

bootcode.bin: $(PRODUCT_DIRECTORY)/bootcode.elf bootcode.stub
	@echo $(WARN_COLOR)OBJ$(NO_COLOR) $@
	@$(OBJCOPY) -O binary $< bootcode.temp
	cat bootcode.stub bootcode.temp > $(PRODUCT_DIRECTORY)/$@

clean:
	@echo $(ERROR_COLOR)CLEAN$(NO_COLOR)
	@-rm -rf ./$(BUILD_DIR)
