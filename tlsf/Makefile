all: libtlsf.a

FPUFLAGS=-mfpu=vfp -mfloat-abi=softfp
ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk ${FPUFLAGS} #-mthumb
ifeq (${CC},vc4-elf-gcc)
CFLAGS=
else
  ifeq (${CC},aarch64-unknown-linux-gnu-gcc)
    CFLAGS=${BASECFLAGS}
  else
    CFLAGS=${ARMCFLAGS}
  endif
endif

libtlsf.a: tlsf.o
	${AR} rcs $@ $^

install: libtlsf.a
	mkdir -p ${out}/{lib,include/tlsf}
	cp tlsf.h ${out}/include/tlsf
	cp libtlsf.a ${out}/lib
