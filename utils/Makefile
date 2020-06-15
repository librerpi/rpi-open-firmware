ramdumper: ramdumper.cc
	${CC} -o $@ $< -lcommon

pv_dumper.o: pv_dumper.cc

pv_dumper: pv_dumper.o map_peripherals.o
	${CC} -o $@ $^ -lcommon -lbcm_host -Wall

pll-inspector.o: pll-inspector.cpp
	${CC} -c -o $@ $< -fpermissive

pll-inspector: pll-inspector.o map_peripherals.o
	${CC} -o $@ $^ -lbcm_host -lcommon

install: ramdumper pv_dumper pll-inspector
	mkdir -pv ${out}/bin
	cp -vi ramdumper ${out}/bin/
	cp -vi pv_dumper ${out}/bin/
	cp -vi pll-inspector ${out}/bin/
