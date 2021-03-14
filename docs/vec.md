based on: https://github.com/raspberrypi/linux/blob/rpi-5.10.y/drivers/gpu/drm/vc4/vc4_vec.c

the VEC deals with generating pal/ntsc video signals

the digital part of VEC is capable of generating component video signals, but all known SoC's are limited to a single DAC

`CM_VECCTL` and `CM_VECDIV` must be used to feed the VEC with a clean 108mhz clock, no fractional divison allowed

# registers
name | addr
--- | ---
`VEC_WSE_RESET` | 0xc0
`VEC_WSE_CONTROL` | 0xc4
`VEC_WSE_WSS_DATA` | 0xc8
`VEC_WSE_VPS_DATA1` | 0xcc
`VEC_WSE_VPS_CONTROL` | 0xd0
`VEC_REVID`           | 0x100
`VEC_CONFIG0`         | 0x104
`VEC_SCHPH`           | 0x108
`VEC_SOFT_RESET`      | 0x10c
`VEC_CLMP0_START`     | 0x144
`VEC_CLMP0_END`       | 0x148
`VEC_FREQ3_2`         | 0x180
`VEC_FREQ1_0`         | 0x184
`VEC_CONFIG1`         | 0x188
`VEC_CONFIG2`         | 0x18c
`VEC_INTERRUPT_CONTROL` | 0x190
`VEC_INTERRUPT_STATUS`  | 0x194
`VEC_FCW_SECAM_B`       | 0x198
`VEC_SECAM_GAIN_VAL`    | 0x19c
`VEC_CONFIG3`           | 0x1a0
`VEC_STATUS0`           | 0x200
`VEC_MASK0`             | 0x204
`VEC_CFG`               | 0x208
`VEC_DAC_TEST`          | 0x20c
`VEC_DAC_CONFIG`        | 0x210
`VEC_DAC_MISC`          | 0x214

`cat /sys/kernel/debug/dri/0/vec_regs` when running `dtoverlay=vc4-kms-v3d` will dump the state of the VEC

## init sequence:
(copied from linux)
```
VEC_WSE_RESET = 1;
VEC_SOFT_RESET = 1;
VEC_WSE_CONTROL = 0;
VEC_SCHPH = 0x28;
VEC_CLMP0_START = 0xac;
VEC_CLMP0_END = 0xec;
VEC_CONFIG2 = VEC_CONFIG2_UV_DIG_DIS | VEC_CONFIG2_RGB_DIG_DIS;
VEC_CONFIG3 = VEC_CONFIG3_HORIZ_LEN_STD;
VEC_DAC_CONFIG = VEC_DAC_CONFIG_DAC_CTRL(0xc) | VEC_DAC_CONFIG_DRIVER_CTRL(0xc) | VEC_DAC_CONFIG_LDO_BIAS_CTRL(0x46);
VEC_MASK0 = 0;
switch (mode)
case NTSC:
  VEC_CONFIG0 = VEC_CONFIG0_NTSC_STD | VEC_CONFIG0_PDEN;
  VEC_CONFIG1 = VEC_CONFIG1_C_CVBS_CVBS;
  break;
case NTSCJ:
  VEC_CONFIG0 = VEC_CONFIG0_NTSC_STD;
  VEC_CONFIG1 = VEC_CONFIG1_C_CVBS_CVBS;
  break;
case PAL:
  VEC_CONFIG0 = VEC_CONFIG0_PAL_BDGHI_STD;
  VEC_CONFIG1 = VEC_CONFIG1_C_CVBS_CVBS;
  break;
case PALM:
  VEC_CONFIG0 = VEC_CONFIG0_PAL_BDGHI_STD;
  VEC_CONFIG1 = VEC_CONFIG1_C_CVBS_CVBS | VEC_CONFIG1_CUSTOM_FREQ;
  VEC_FREQ3_2 = 0x223b;
  VEC_FREQ1_0 = 0x61d1;
  break;
}
VEC_DAC_MISC = VEC_DAC_MISC_VID_ACT | VEC_DAC_MISC_DAC_RST_N;
VEC_CFG = VEC_CFG_VEC_EN;
```

video timing for [pixel valve](pixelvalve.md):
param | ntsc | pal
--|--|--
hactive | 720 | 720
hfp     | 14  | 20
hsync   | 64  | 64
hbp     | 60  | 60
vactive | 240 | 288
vfp     | 3/4 | 2
vsync   | 3   | 3
vbp     | 16  | 20/19

## `VEC_WSE_RESET`
writing a 1 to this register will reset the WSE block
## `VEC_WSE_CONTROL`
write 0 to disable the CGSM-A and WSE blocks
## `VEC_SCHPH`
```
Color subcarrier phase: phase = 360 * SCHPH / 256.
0x28 <=> 39.375 deg.
```
yes, the math does not agree with the example, linux always sets it to 0x28
## `VEC_SOFT_RESET`
writing a 1 to this register will reset the VEC block
