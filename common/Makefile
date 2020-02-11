all: libcommon.a

FPUFLAGS=-mfpu=vfp -mfloat-abi=hard
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk ${FPUFLAGS}
BASECFLAGS=-Ilib -I. -Iinclude -nostdlib -nostartfiles -ffreestanding -ffunction-sections -Wall -g
ifeq (${CC},vc4-elf-gcc)
CFLAGS=${BASECFLAGS}
else
  ifeq (${CC},aarch64-unknown-linux-gnu-gcc)
    CFLAGS=${BASECFLAGS}
  else
    CFLAGS=${BASECFLAGS} ${ARMCFLAGS}
  endif
endif
CXXFLAGS=${CFLAGS} -fno-exceptions -fno-rtti
LFLAGS=-nostdlib -nostartfiles -ffreestanding

libcommon.a: panic.o udelay.o cxx_runtime.o hardware.o print_timestamp.o BCM2708Gpio.o IODevice.o
	${AR} rcs $@ $^

install: libcommon.a
	mkdir -pv ${out}/lib
	cp -r include/ ${out}/
	cp -r hardware.h broadcom ${out}/include/
	cp libcommon.a ${out}/lib/
