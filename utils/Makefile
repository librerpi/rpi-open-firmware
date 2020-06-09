ramdumper: ramdumper.cc
	${CC} -o $@ $< -lcommon

install: ramdumper
	mkdir -pv ${out}/bin
	cp -vi ramdumper ${out}/bin/
