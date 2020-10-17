SRCS = \
	start.S \
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

.PHONY: default all clean device

default: build/bootcode.bin start4.elf

BASE_OBJ := $(addprefix build/, $(addsuffix .o, $(basename $(SRCS))))
EXTRA_OBJ := arm_chainloader.o
OBJ := $(BASE_OBJ) $(EXTRA_OBJ)

include $(BASE_OBJ:.o=.d)

LINKFLAGS = -nostdlib -nostartfiles -Wl,--build-id=none -T linker.ld -Wl,--no-omagic --entry=_start -Wl,--gc-sections # -Wl,--cref

CFLAGS = -c -nostdlib -Wno-multichar -std=c11 -fsingle-precision-constant -Wdouble-promotion -D__VIDEOCORE4__ -I./vc4_include/ -I./ -Wall -Werror=return-type -fno-common \
	 -Wmissing-prototypes -Waggressive-loop-optimizations -Werror -ffunction-sections -g -ftls-model=initial-exec
ASFLAGS = -c -nostdlib -x assembler-with-cpp -D__VIDEOCORE4__ -I./vc4_include/ -I./
CXXFLAGS = -c -nostdlib -Wno-multichar -std=c++11 -fno-exceptions -fno-rtti -D__VIDEOCORE4__ -I./vc4_include/ -I./ -Werror=return-type -ffunction-sections

CREATE_SUBDIR = \
	@DIR="$(dir $@)"; \
	if [ ! -d $$DIR ]; then mkdir -p $$DIR; fi

# generate the dep files
build/%.d: %.s
	$(CREATE_SUBDIR)
	@echo CC -M $< -o $@
	@$(CC) $(ASFLAGS) -MM -o $@ $<
	@sed 's,\($*\)\.o[ :]*,build/\1.o $@ : ,g' -i $@
build/%.d: %.c
	$(CREATE_SUBDIR)
	@echo CC -M $< -o $@
	@$(CC) $(CFLAGS) -MM -o $@ $<
	@sed 's,\($*\)\.o[ :]*,build/\1.o $@ : ,g' -i $@
build/%.d: %.cc
	$(CREATE_SUBDIR)
	@echo CXX -M $< -o $@
	@$(CXX) $(CXXFLAGS) -MM -o $@ $<
	@sed 's,\($*\)\.o[ :]*,build/\1.o $@ : ,g' -i $@

#
# rules to build c/asm files.
#
build/%.o: %.c
	$(CREATE_SUBDIR)
	@echo CC $@
	@$(CC) $(CFLAGS) $< -o $@

build/%.o: %.cc
	$(CREATE_SUBDIR)
	@echo CXX $@
	@$(CXX) $(CXXFLAGS) $< -o $@

build/%.o: %.S
	$(CREATE_SUBDIR)
	@echo CC $@
	@$(CC) $(ASFLAGS) $< -o $@

arm_chainloader.o: arm_chainloader/build/arm_chainloader.bin
	$(CREATE_SUBDIR)
	$(OBJCOPY) -I binary -O elf32-vc4 -B vc4 $< $@

build/arm_chainloader.d:
	@echo 'build/arm_chainloader.o: arm_chainloader/build/arm_chainloader.bin' > $@

startelf4.o: startelf4.S
	$(CC) startelf4.S -o startelf4.o -c

start4.elf: startelf4.o startelf.o pll_read.o build/traps.o trap.o hang_cpu.o arm_monitor.o panic.o interrupt.o utils.o utils4.o drivers/BCM2708ClockDomains.o pl011.o BCM2708PlatformStartup.o drivers/BCM2708PowerManagement.o drivers/BCM2708ArmControl.o arm_chainloader.o otp.o build/otp_asm.o
	$(CC) -o $@ $^ start.ld -nostdlib -lcommon

build/bootcode.elf: $(OBJ)
	$(CREATE_SUBDIR)
	@echo LD $@
	@$(CC) $(LINKFLAGS) -Wl,-Map=build/bootcode.map $(OBJ) -o $@ -lcommon -lc -lgloss -lgcc

build/bootcode.bin: build/bootcode.elf
	$(CREATE_SUBDIR)
	@echo OBJ $@
	@$(OBJCOPY) -O binary $< $@

clean:
	@echo CLEAN
	@-rm -rf build
