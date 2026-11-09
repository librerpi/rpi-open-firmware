# ARM Linux handoff with an LK framebuffer

The ARM chainloader can preserve a framebuffer configured by the LK VPU
firmware and describe it to Linux as a `simple-framebuffer`. LK is responsible
for initializing the display and supplying the optional metadata; this
repository does not initialize the DSI panel. The integration requires an LK
build with the matching handoff and the BCM2836/2837 local-peripheral window
mapped at ARM physical `0x40000000` for Linux SMP.

## Build

With Nix and an `arm-none-eabi` toolchain available:

```sh
make static-test
nix-build -A arm.chainloader -o result-chainloader
```

The chainloader binary is `result-chainloader/arm_chainloader.bin`. The LK
build embeds that binary; follow the matching LK project's instructions to
build stage 1 and stage 2. `make test` runs the static checks and a Nix
chainloader build without creating a `result` link. These checks do not boot
the firmware or validate the Linux device tree on hardware.

The tested two-stage arrangement places the FAT stage-1 image at
`/bootcode.bin` and the LK stage-2 ELF at `/boot/lk.elf` on the boot
partition. The ARM chainloader reads `zImage`, `cmdline.txt`, and `rpi3.dtb`
from the FAT partition; `bcm2710-rpi-3-b.dtb` is also accepted if `rpi3.dtb`
is absent. An `initrd` file is optional. The tested Linux command line uses
`clk_ignore_unused pd_ignore_unused` to preserve the firmware-configured
display pipeline. Native Linux display drivers must not simultaneously take
ownership of that pipeline.

## Firmware-data contract

The VPU copies the ARM payload to ARM physical address zero. The first five
32-bit firmware-data words begin at offset `0x20` and retain their original
layout: SDRAM size, VPU CPU ID, serial, board revision, and UART clock. The
chainloader reserves ten additional words after this prefix, before any ARM
exception-handler code. Producers must not write those words into older
payloads that do not reserve them.

When the word at offset `0x34` contains the `LFB1` magic
`0x3142464c`, the following words describe the framebuffer pool, the primary,
back and active physical addresses, width, height, stride in pixels and
format. The field indices are defined in `common/hardware.h`. The supported
layout is a 3 MiB pool at `0x01800000`, with 800×480 ARGB8888 buffers at
`0x01800000` and `0x01977000`. Linux receives a 3200-byte stride. The
chainloader reserves the pool with `no-map` and adds a root-level
`simple-framebuffer` node for the active buffer.

Without the magic, the optional framebuffer and LK-specific device-tree
fixups are skipped. With the magic, the boot firmware must already have
initialized the display, USB power and PHY, and routed PL011 to GPIO14/15.
The device tree must provide the matching UART pin group and phandle.

## Validation and limitations

A Raspberry Pi 3B with the official 7-inch DSI display was booted using the
LK fixture and a chainloader built from the preceding tested implementation.
The capture showed all four CPUs online, Linux simplefb/fbcon, USB keyboard
enumeration, and a Buildroot login prompt; the tester reported a successful
console login. The LK mapping and display initialization are cross-repository
dependencies and the clean LK patch series is not yet complete.

The subsequent change that scopes LK-specific device-tree fixups to the
optional handoff has been built but has not yet been tested on hardware.
Raspberry Pi 1 and Pi 2 compatibility and repeat-boot reliability also remain
unverified. The tested Linux boot reported a `dsi0p` clock-parent warning;
clock and power ownership are not solved by the command-line workarounds.
