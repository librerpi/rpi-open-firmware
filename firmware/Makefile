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
	drivers/BCM2708PowerManagement.cc \
	drivers/BCM2708UsbPhy.cc \
	drivers/BCM2708ArmControl.cc \
	drivers/BCM2708ClockDomains.cc \
	drivers/gpclk.cc \
	hang_cpu.o \
	utils.cc \
	pl011.c \
	pll_read.c \
	interrupt.c \
	traps.S \
	otp.c \
	otp_asm.S \
	BCM2708PlatformStartup.cc

ARCH = vc4

BUILD_DIR = build
TARGET_BUILD_DIR = $(BUILD_DIR)/$(ARCH)-objects
PRODUCT_DIRECTORY = $(BUILD_DIR)

NO_COLOR=""
OK_COLOR=""
ERROR_COLOR=""
WARN_COLOR=""

.PHONY: default all clean device

default: $(PRODUCT_DIRECTORY)/bootcode.bin start4.elf

EXTRA_OBJ := arm_chainloader.o
OBJ := $(addprefix $(TARGET_BUILD_DIR)/, $(addsuffix .o, $(basename $(SRCS)))) $(EXTRA_OBJ)

# the cross compiler should already be in your path
CROSS_COMPILE ?= vc4-elf-
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AS = $(CC)
OBJCOPY = $(CROSS_COMPILE)objcopy
LINKFLAGS = -nostdlib -nostartfiles -Wl,--build-id=none -T linker.ld -Wl,--no-omagic --entry=_start -Wl,--gc-sections # -Wl,--cref

CFLAGS = -c -nostdlib -Wno-multichar -std=c11 -fsingle-precision-constant -Wdouble-promotion -D__VIDEOCORE4__ -I./vc4_include/ -I./ -Wall -Werror=return-type -fno-common \
	 -Wmissing-prototypes -Waggressive-loop-optimizations -Werror -ffunction-sections -g
ASFLAGS = -c -nostdlib -x assembler-with-cpp -D__VIDEOCORE4__ -I./vc4_include/ -I./
CXXFLAGS = -c -nostdlib -Wno-multichar -std=c++11 -fno-exceptions -fno-rtti -D__VIDEOCORE4__ -I./vc4_include/ -I./ -Werror=return-type -ffunction-sections

HEADERS := \
	$(shell find . -type f -name '*.h') \
	$(shell find . -type f -name '*.hpp')

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

startelf4.o: startelf4.S
	vc4-elf-gcc startelf4.S -o startelf4.o -c

start4.elf: startelf4.o startelf.o pll_read.o traps.o trap.o hang_cpu.o arm_monitor.o panic.o interrupt.o utils.o utils4.o drivers/BCM2708ClockDomains.o pl011.o BCM2708PlatformStartup.o drivers/BCM2708PowerManagement.o drivers/BCM2708ArmControl.o arm_chainloader.o otp.o otp_asm.o
	$(CC) -o $@ $^ start.ld -nostdlib -lcommon

.PRECIOUS: $(OBJ)

$(PRODUCT_DIRECTORY)/bootcode.elf: $(OBJ)
	mkdir -p $(TARGET_BUILD_DIR)
	mkdir -p $(PRODUCT_DIRECTORY)
	@echo $(WARN_COLOR)LD  $(NO_COLOR) $@
	@$(CC) $(LINKFLAGS) -Wl,-Map=$(PRODUCT_DIRECTORY)/bootcode.map $(OBJ) -o $@ -lcommon -lc -lgloss -lgcc

bootcode.stub:
	truncate bootcode.stub -s 512

$(PRODUCT_DIRECTORY)/bootcode.bin: $(PRODUCT_DIRECTORY)/bootcode.elf
	@echo $(WARN_COLOR)OBJ$(NO_COLOR) $@
	@$(OBJCOPY) -O binary $< $@

clean:
	@echo $(ERROR_COLOR)CLEAN$(NO_COLOR)
	@-rm -rf ./$(BUILD_DIR)
