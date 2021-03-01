SRCS = \
	start.s \
	mmu_asm.s \
	lib_armv6/arm_bcopy.s \
	lib_armv6/arm_bzero.s \
	lib_armv6/arm_locore.s \
	lib_armv6/arm_strlen.s \
	lib_armv6/arm_memcmp.s \
	lib_armv6/string_misc.c \
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

.PHONY: default all clean

default: build/arm_chainloader.bin

OBJ := $(addprefix build/, $(addsuffix .o, $(basename $(SRCS))))

include $(OBJ:.o=.d)

LINKFLAGS = -flto -nostdlib -march=$(ARCH) -Wl,--build-id=none -T linker.lds -ltlsf -lgcc -lcommon -lnotc -lstdc++ -Wl,--gc-sections -ltlsf
FPUFLAGS=-mfpu=vfp -mfloat-abi=softfp

# applied to as, gcc and g++
COMMON_FLAGS = -flto -c -ffreestanding -march=$(ARCH) -I../ -I./ -I./drivers/libfdt/ -mtune=arm1176jzf-s ${FPUFLAGS} -ffunction-sections -fdata-sections -DBAREMETAL -fno-common

# applied to gcc and g++
COMMON_FLAGS2 = -nostdlib -nostartfiles -g -O3
CFLAGS = $(COMMON_FLAGS) $(COMMON_FLAGS2) -std=c11 -Wdeclaration-after-statement
CXXFLAGS = $(COMMON_FLAGS) $(COMMON_FLAGS2) -std=c++11 -fno-exceptions -fno-rtti
ASFLAGS = $(COMMON_FLAGS) -x assembler-with-cpp

CREATE_SUBDIR = \
	@DIR="$(dir $@)"; \
	if [ ! -d $$DIR ]; then mkdir -p $$DIR; fi

build/mmu_asm.o: mmu_asm.s
	@echo $(WARN_COLOR)CC  $(NO_COLOR) $@ #-mthumb
	@$(CC) $(CFLAGS) $< -o $@ -march=armv7-a -mtune=cortex-a7

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
	@echo $(WARN_COLOR)CC  $(NO_COLOR) $@
	$(CC) $(CFLAGS) $< -o $@

build/%.o: %.cc
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)CXX $(NO_COLOR) $@ #-mthumb
	$(CXX) $(CXXFLAGS) $< -o $@ #-mthumb

build/%.o: %.s
	$(CREATE_SUBDIR)
	@echo $(WARN_COLOR)AS  $(NO_COLOR) $@
	@$(CC) $(ASFLAGS) $< -o $@

build/arm_chainloader.bin: build/arm_chainloader.elf
	@echo $(WARN_COLOR)OBJ$(NO_COLOR) $@
	@$(OBJCOPY) -O binary $< $@

build/arm_chainloader.elf: $(OBJ)
	@echo $(WARN_COLOR)LD  $(NO_COLOR) $@
	$(CC) $(OBJ) $(LINKFLAGS) -o $@ -Wl,-Map=build/arm_chainloader.map

clean:
	@echo $(ERROR_COLOR)CLEAN$(NO_COLOR)
	@-rm -rf ./build
