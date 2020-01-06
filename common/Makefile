all: libcommon.a

FPUFLAGS=-mfpu=vfp -mfloat-abi=hard
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk ${FPUFLAGS}
BASECFLAGS=-Ilib -I. -Iinclude -nostdlib -nostartfiles -ffreestanding
ifeq (${CC},vc4-elf-gcc)
CFLAGS=${BASECFLAGS}
 else
CFLAGS=${BASECFLAGS} ${ARMCFLAGS}
endif
CXXFLAGS=${CFLAGS} -g -fno-exceptions
LFLAGS=-nostdlib -nostartfiles -ffreestanding

libcommon.a: xprintf.o panic.o udelay.o cxx_runtime.o printf.o putchar.o puts.o hardware.o print_timestamp.o
	${AR} rcs $@ $^

install: libcommon.a
	mkdir -pv ${out}/lib
	cp -r include/ ${out}/
	cp -r hardware.h broadcom ${out}/include/
	cp libcommon.a ${out}/lib/
