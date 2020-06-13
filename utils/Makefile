ramdumper: ramdumper.cc
	${CC} -o $@ $< -lcommon

pv_dumper: pv_dumper.cc
	${CC} -o $@ $< -lcommon -lbcm_host -Wall

install: ramdumper pv_dumper
	mkdir -pv ${out}/bin
	cp -vi ramdumper ${out}/bin/
	cp -vi pv_dumper ${out}/bin/
