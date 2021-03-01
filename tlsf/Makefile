all: libtlsf.a

CFLAGS=-ffunction-sections -fdata-sections

ifeq (${CC},vc4-elf-gcc)
  CFLAGS=
else
  ifeq (${CC},aarch64-unknown-linux-gnu-gcc)
    CFLAGS +=
  else
    CFLAGS += -mtune=arm1176jzf-s -march=armv6zk -mfpu=vfp -mfloat-abi=softfp
  endif
endif

libtlsf.a: tlsf.o
	${AR} rcs $@ $^

install: libtlsf.a
	mkdir -p ${out}/{lib,include/tlsf}
	cp tlsf.h ${out}/include/tlsf
	cp libtlsf.a ${out}/lib
