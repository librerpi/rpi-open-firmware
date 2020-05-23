all: libnotc.a

BASECFLAGS=-Iinclude -g
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk -mfpu=vfp -mfloat-abi=softfp #-mthumb
ifeq (${CC},vc4-elf-gcc)
  CFLAGS=${BASECFLAGS}
else
  ifeq (${CC},aarch64-unknown-linux-gnu-gcc)
    CFLAGS=${BASECFLAGS}
  else
    CFLAGS=${BASECFLAGS} ${ARMCFLAGS}
  endif
endif

libnotc.a: xprintf.o puts.o putchar.o printf.o
	${AR} rcs $@ $^

install: libnotc.a
	mkdir -pv ${out}/lib
	cp libnotc.a ${out}/lib
	cp -r include ${out}
