all: arm64.elf arm64.bin

CFLAGS = -nostdlib -nostartfiles -ffreestanding -g

arm64.elf: start.o main.o
	$(LD) $^ -o $@ -Map=arm64.map -g -T stub.ld

arm64.bin: arm64.elf
	$(OBJCOPY) -O binary $< $@

arm64.S: arm64.elf
	$(OBJDUMP) -d $< > $@

install: arm64.elf arm64.bin arm64.map arm64.S
	mkdir -pv ${out}
	cp -vi $^ ${out}
