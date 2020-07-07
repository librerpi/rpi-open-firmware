all: libcommon.a

FPUFLAGS=-mfpu=vfp -mfloat-abi=softfp
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk
CFLAGS = -Ilib -I. -Iinclude -ffunction-sections -Wall -g

OBJS=hexdump.o crc32.o

ifeq (${BAREMETAL},1)
  OBJS += hardware.o panic.o cxx_runtime.o print_timestamp.o IODevice.o udelay.o BCM2708Gpio.o
  CFLAGS += -nostdlib -nostartfiles -ffreestanding -DBAREMETAL
  ARMCFLAGS += -mthumb ${FPUFLAGS}
else
  CFLAGS2=${BASECFLAGS}
endif

ifeq (${CC},vc4-elf-gcc)
else
  ifeq (${CC},aarch64-unknown-linux-gnu-gcc)
  else
    CFLAGS += ${ARMCFLAGS}
  endif
endif
CXXFLAGS=${CFLAGS} -fno-exceptions -fno-rtti
LFLAGS=-nostdlib -nostartfiles -ffreestanding


libcommon.a: ${OBJS}
	${AR} rcs $@ $^

install: libcommon.a
	mkdir -pv ${out}/lib
	cp -r include/ ${out}/
	cp -r hardware.h broadcom ${out}/include/
	cp libcommon.a ${out}/lib/
