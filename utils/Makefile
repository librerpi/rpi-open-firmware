CFLAGS := -g `${PKG_CONFIG} --cflags libdrm`
CXXFLAGS := ${CFLAGS}

ramdumper: ramdumper.cc
	${CC} -o $@ $< -lcommon

pv_dumper.o: pv_dumper.cc

pv_dumper: pv_dumper.o map_peripherals.o
	${CC} -o $@ $^ -lcommon -lbcm_host -Wall ${CFLAGS}

pll-inspector.o: pll-inspector.cpp
	${CC} -c -o $@ $< -fpermissive

pll-inspector: pll-inspector.o map_peripherals.o
	${CC} -o $@ $^ -lbcm_host -lcommon

kms-test: kms-test.o
	${CXX} -o $@ $^ -ldrm

install: ramdumper pv_dumper pll-inspector kms-test
	mkdir -pv ${out}/bin
	cp -vi $^ ${out}/bin/
