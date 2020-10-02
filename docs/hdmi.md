from `linux/drivers/gpu/drm/vc4/vc4_hdmi_regs.h` and `linux/drivers/gpu/drm/vc4/vc4_hdmi_phy.c`
# HDMI register overview
pi4 base for each set and port
reg set | HDMI0       | HDMI1       | length
--------|-------      |-------      |---
INTR2   | 0x7ef00100  | 0x7ef00100  | 0x30
CSC     | 0x7ef00200  | 0x7ef00280  | 0x80
DVP     | 0x7ef00300  | 0x7ef05300  | 0x200
HDMI    | 0x7ef00700  | 0x7ef05700  | 0x300
PHY     | 0x7ef00f00  | 0x7ef05f00  | 0x80
RM      | 0x7ef00f80  | 0x7ef05f80  | 0x80
PACKET  | 0x7ef01b00  | 0x7ef06b00  | 0x200
METADATA| 0x7ef01f00  | 0x7ef06f00  | 0x400
CEC     | 0x7ef04300  | 0x7ef09300  | 0x100
HD      | 0x7ef20000  | 0x7ef20000  | 0x100

# HDMI RM registers
register | offset
---|---
CONTROL | 0x0
OFFSET | 0x18
FORMAT | 0x1c
## CONTROL
bits | usage
-----|---
4    | free run
17   | enable load integrator
19   | enable freeze counters
## OFFSET
bits | usage
-----|---
0:30 | offset 9.22 fixed-point
## FORMAT
bits | usage
-----|---
24:25| shift
# HDMI PHY registers
VC4 only has 2 phy registers, and they live within `hdmi_base+0x200`

VC5 has many phy registers, in their own definition in the dts

register | VC4 | VC5
---------|-----|---
`RESET_CTL`     | 0x2c0 | 0x0
`POWERDOWN_CTL` |       | 0x4
`CTL_0`         | 0x2c4 | 0x8
`CTL_1`         |       | 0xc
`CTL_2`         |       | 0x10
`CTL_3`         |       | 0x14
`PLL_CTL_0`     |       | 0x1c
`PLL_CTL_1`     |       | 0x20
`CLK_DIV`       |       | 0x28
`PLL_CFG`       |       | 0x34
`TMDS_CLK_WORD_SEL`|    | 0x44
`CHANNEL_SWAP`  |       | 0x4c
`PLL_CALIBRATION_CONFIG_1`| | 0x50
`PLL_CALIBRATION_CONFIG_2`| | 0x54
`PLL_CALIBRATION_CONFIG_4`| | 0x5c
## reset control
bits | usage
-----|---
0    | tx0 reset
1    | tx1 reset
2    | tx2 reset
3    | clock reset
4    | pll divisor resetb
5    | pll resetb
## powerdown control
## control 0
preemp control for each of the 4 physical channels, not affected by the swap below, so you must swap the termination fields yourself

see `vc5_hdmi_phy_settings` in linux for values, they will vary based on `tmds_rate`
bits | usage
-----|---
0:4  | "clock" maindrv
5:7  | "clock" preemp
8:12 | "dat0" maindrv
13:15| "dat0" preemp
16:20| "dat1" maindrv
21:23| "dat1" preemp
24:28| "dat2" maindrv
29:31| "dat2" preemp
## control 1
unknown purpose, but `vc5_hdmi_phy_settings` in linux shows the correct value for each `tmds_rate`

not affected by the swap below, so you must swap the termination fields yourself

bits | usage
-----|---
0:4  | res select "clock"
5:9  | res select "dat0"
10:14| res select "dat1"
15:19| res select "dat2"

## control 2
primarily controls the termination resistors on each physical channel, not affected by the swap below, so you must swap the termination fields yourself
bits | usage
-----|---
0:3  | "clock" channel
4:7  | "dat0" channel
8:11 | "dat1" channel
12:15| "dat2" channel
16:19| vco gain

## control 3
bits | usage | set to X by linux
-----|-------|---
0:5  | icp   | 0x18 or 0x1c based on `vco_freq`
6:7  | cz    | 3
8:9  | cp    | 1
10:11| cp1   | 1
12:16| rz    | 6
17:19| rp    | 4
## pll control 0
bits | usage
-----|---
5    | enable vco clock
6    | vco cont en
7    | vco post div2
8    | vco fb div2
9    | vco select
10   | emulate vc high
11   | emulate vc low
12   | vc range enable
13   | mash11 mode
## pll control 1
bits | usage
-----|---
11:12| post reset sel
13   | freq doubler enable
14:15| freq doubler delay
16:27| cpp
## clock divisor
bits | usage
-----|---
8:15 | vco
## pll config
bits | usage
-----|---
0:3  | pdiv
## tmds clock word select
set to 0 or 3, based on pixel clock
## channel swap
allows the 4 differential lanes on the PHY to map to any of the 4 hdmi channels, for simpler PCB routing
bits | usage
-----|---
0:1  | tx0
4:5  | tx1
8:9  | tx2
12:13| tx clock

0=lane0, 1=lane1, 2=lane2, 3=clock-lane

HDMI0 should be set to 0x3210 on a pi4B

HDMI1 should be set to 0x2301 on a pi4B, since the physical hdmi connector has some lanes swapped
## pll calibration config 1
bits | usage
-----|---
0:27 | min limit
## pll calibration config 2
bits | usage
-----|---
0:27 | max limit
## pll calibration config 4
bits | usage           | set to X by linux
-----|-----------------|-----
0:15 | hold threshold  | 0xe147
16:31| stable theshold | 0xe14

# HDMI HDMI registers
register | offset
---|---
`FIFO_CTL`            | 0x74
`AUDIO_PACKET_CONFIG` | 0xb8
`RAM_PACKET_CONFIG`   | 0xbc
`RAM_PACKET_STATUS`   | 0xc4
`CRP_CFG`             | 0xc8
`CTS_0`               | 0xcc
`CTS_1`               | 0xd0
`SCHEDULER_CONTROL`   | 0xe0
`HORZA`               | 0xe4
`HORZB`               | 0xe8
`VERTA0`              | 0xec
`VERTB0`              | 0xf0
`VERTA1`              | 0xf4
`VERTB1`              | 0xf8
`MAI_CHANNEL_MAP`     | 0x9c
`MAI_CONFIG`          | 0xa0
`HOTPLUG`             | 0x1a8
