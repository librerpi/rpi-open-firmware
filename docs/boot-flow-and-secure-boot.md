# boot stages

- stage-0, mask rom, runs on VPU immediately upon coming out of reset, and loads stage-1
- stage-1, `bootcode.bin`, runs on VPU, deals with ram-init and loading stage-2
- stage-2, `start.elf` and `start4.elf`, runs on VPU, provides the bulk of the runtime services, and loads stage-3
- stage-3, `kernel.img`, runs on the arm core, as either the main os or the arm side bootloader

# pi0-pi2 stage-0
by default, supports loading from 2 sources:
- `bootcode.bin` on the fat32 partition of an SD card
- receiving a `bootcode.bin` blob over usb, via the rpiboot protocol, if usb device mode is possible
3 other modes are possible and partially documented
- nand flash
- spi flash
- i2c slave mode

# pi3 stage-0
supports everything found on pi0-pi2, with 1 addition
- usb-host mode, loading `bootcode.bin` from either an MSD or over tftp with the onboard usb NIC

# bcm2711 stage-0
only supports booting from 3 targets
- `recovery.bin` on a fat32 partition on an SD card, via either GPT or MBR
- SPI flash
- rpiboot protocol, as with previous models

# VC4(pi0-pi3) stage-1
3 options exist at this stage
- the closed `bootcode.bin`
- the rpi-open-firmware `bootcode.bin` contains a complete stage-1 and stage-3 bootloader, skips stage-2
- the lk-overlay `bootcode.bin` target, provides many modular options, including ram-init and loading a stage-2

# VC4(pi0-pi3) stage-2
3 options exist at this stage
- the closed `start.elf` and its variants
- the closed `msd.elf`, makes the pi emulate a mass-storage device, and expose the SD/eMMC over usb
- the lk-overlay `.elf` target, provides many modular options, including bringing the 2d subsystem+ntsc online, and running a stage-3 on the arm core

# VC4(pi0-pi3) stage-3
many options exist at this state, but which ones work depend on the stage-2 used
- u-boot currently relies on the closed `start.elf` to work, needs to be re-ported
- https://github.com/pftf/RPi3 currently relies on the closed `start.elf`
- rpi-open-firmware has an arm32 bootloader that works on pi0-pi3
- lk-overlay has an arm32 bootloader that currently only works on pi2

# bcm2711 stage-1
3 options currently exist
- the closed `recovery.bin`, exclusively used for re-flashing the SPI chip, booted from either the SD card or rpiboot
- the closed `bootcode.bin` held in SPI flash, deals with ram-init, and loading stage-2
- the lk-overlay `recovery.bin` target, some modular options, but no ram-init, so your limited to 128kb of L2 cache

# bcm2711 stage-2
3 options currently exist
- the closed `start4.elf` and its variants
- the closed `msd4.elf`, does the same job as `msd.elf` on previous models
- the lk-overlay bcm2711 `.elf` target, more modular options, but no arm bringup yet

# secure-boot options

pi0-pi3 and bcm2711B0T all support hmac-sha1 based signing
a 20 byte key exists in ram, and gets XOR'd with a 16 byte key held in OTP(one time programmable)
pi0-pi3, the signature must be in a `bootcode.sig` file
bcm2711B0T, the 20 byte signature is just appended to the `recovery.bin`

bcm2711B1T and bcm2711C0T, RSA support has been added, but not yet investigated

the closed bcm2711 `bootcode.bin` is signed by a broadcom keypair(todo, verify this), and will then validate everything else against a user-chosen keypair
the sha256 of the user-chosen keypair can be burned into OTP to lock it in
the user-chosen pubkey must be stored in the SPI flash
with just `SIGNED_BOOT=1`, you only need `boot.img` + `boot.sig`
with signing enabled in OTP, `SIGNED_BOOT=1` isnt required, but `bootconf.txt` must be signed with `bootconf.sig`, along with `boot.img` + `boot.sig`

the bcm2711 stage-2 `start4.elf` does not need to support signature validation, the stage-1 will load `boot.img` into ram, verify it, and pass it as a ramdisk to the `stage-2`
