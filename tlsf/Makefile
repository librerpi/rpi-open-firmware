all: libtlsf.a

ARMCFLAGS=-mtune=arm1176jzf-s -march=armv6zk
ifeq (${CC},vc4-elf-gcc)
CFLAGS=
 else
CFLAGS=${ARMCFLAGS}
endif

libtlsf.a: tlsf.o
	${AR} rcs $@ $^

install: libtlsf.a
	mkdir -p ${out}/{lib,include/tlsf}
	cp tlsf.h ${out}/include/tlsf
	cp libtlsf.a ${out}/lib
