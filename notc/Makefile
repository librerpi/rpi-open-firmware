all: libnotc.a

BASECFLAGS=-Iinclude
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk -mfpu=vfp -mfloat-abi=hard
ifeq (${CC},vc4-elf-gcc)
  CFLAGS=${BASECFLAGS}
else
  CFLAGS=${BASECFLAGS} ${ARMCFLAGS}
endif

libnotc.a: xprintf.o puts.o putchar.o printf.o
	${AR} rcs $@ $^

install: libnotc.a
	mkdir -pv ${out}/lib
	cp libnotc.a ${out}/lib
	cp -r include ${out}
