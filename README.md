# Minimal Raspberry Pi VPU firmware

`rpi-open-firmware` is a small firmware for the [RPi VPU](https://en.wikipedia.org/wiki/VideoCore), capable of initializing UART, VPU PLL (PLLC), and ARM itself. It's intended as a libre replacement for the stock `bootcode.bin` normally loaded from the SD card. A UART cable is highly recommended for the time being, though not strictly necessary. Additionally, there is a second-stage chainloader running on ARM capable of initializing eMMC, FAT, and the Linux kernel.

See [issue #31](https://github.com/christinaa/rpi-open-firmware/issues/31) for a compatibility table and related discourse.

Technical discussion occurs in `#raspberrypi-internals` on [libera](https://web.libera.chat/). Please come on IRC before randomly hacking on the codebase :-)

All Broadcom headers are licensed under 3-Clause BSD License while our code is under GPLv2+. See `LICENSE` for more information. Various external projects are mirrored under a mix of GPL-compatible licenses.

## Building

As a prerequisite, Julian Brown's [VC4 toolchain](https://github.com/itszor/vc4-toolchain) is necessary as well as the `arm-none-eabi-` toolchain (Debian package `gcc-arm-none-eabi`). You can tweak the VC4 toolchain path in `CROSS_COMPILE` in `Makefile` and the ARM path in `arm_chainloader/Makefile` if necessary. Contributors should not commit their personal paths. After configuration, run `buildall.sh`. The binary is at `build/bootcode.bin`, ready to be copied to an SD card.

### Building on macOS

macOS compilation is similar to GNU/Linux, save platform errata described here. Instructions to build the ARM toolchain are [here](https://launchpadlibrarian.net/287100910/How-to-build-toolchain.pdf). Due to symlinking by default, GCC must be installed manually, and, an older version of guile is necessary (homebrew packages `gcc-6` and `guile18`, respectively). Finally, set the environment variable `LIBRARY_PATH` to `/lib:/lib64` when running `buildall.sh. 

## Technical Details
The firmware is split into two parts, one running on the VC4 and the other on ARM. The VC4 part initializes PLLC and moves VPU over to it, and then brings up UART. It performs SDRAM initialization, mapping it to  `0xC0000000` (uncached alias). Next, ARM is initialized, and the embedded bootloader is mapped to ARM address `0x0`. `arm_chainloader` is then executed.

The ARM chainloader initializes the eMMC controller and accesses the FAT boot partition. From here, it chainloads the Linux kernel (other payloads likely do not work due to mailbox usage).

## Does it boot Linux?

Yes, with some conditions. You can boot a very minimal version of Linux without the firmware and get it to work with UART and eMMC. Support for USB, DMA, and Ethernet are in the works, which will be sufficient for certain headless systems. Still, many other peripherals require bringup, such as video. Additionally, drivers for power management need to written.

## Thanks To
 * **[Herman Hermitage](https://github.com/hermanhermitage)** for his VC4 documentation and for helping determine suitable ARM PLL configurations.
 * **[Julian Brown](https://github.com/itszor)** for reviewing the code and for his awesome VC4 toolchain.
 * **[Alyssa Rosenzweig](https://github.com/bobbybee)** for her contributions to the firmware especially in areas of Linux bringup and early ARM side initialization, as well as fixing mailbox support.
 * **[David Given](https://github.com/davidgiven)** for his initial LLVM project used as the base for the initial LLVM toolchain before moving to GCC.
 * **[Scott Mansell](https://github.com/phire)** for reviewing the code.
 * **[Broadcom](https://github.com/broadcom)** for their header release.
 * Various other people not mentioned here.
