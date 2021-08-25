# registers

name | VC4 addr
---|---
`SCALER_DISPSTAT`   | `0x7e400004`
`SCALER_DISPDITHER` | `0x7e400014`
`SCALER_DISPEOLN`   | `0x7e400018`
`SCALER_DISPBKGND0` | `0x7e400044`
`SCALER_DISPBKGND1` | `0x7e400054`
`SCALER_DISPBKGND2` | `0x7e400064`
`SCALER_DISPGAMADR` | `0x7e400078`
`SCALER_DISPGAMDAT` | `0x7e4000e0`

## `SCALER_DISPSTAT`
bits | usage
---|---
0     | IRQSCL
1:3   | IRQDISP(0:2)
4     | IRQDMA, Set when any of the EOF/EOLN/ESFRAME/ESLINE bits are set and their corresponding interrupt bit is enabled in DISPCTRL.
5     | IRQSLVWR, Set when `SCALER_DISPSTAT_DMA_ERROR` is set, or `SCALER_DISPSTAT_RESP_ERROR` is not `SCALER_DISPSTAT_RESP_OKAY`.
6     | IRQSLVRD, Set on AXI slave write decode error
7     | `DMA_ERROR`, Set on AXI slave read decode error
8     | eof(0)
9     | euflow(0)
10    | esline(0)
11    | esframe(0)
12    | eoln(0)
13    | coblow(0)
14:15 | resp, 0=okay, 1=EXOKAY, 2=SLVERR, 3=DECERR
16    | eof(1)
17    | euflow(1)
18    | esline(1)
19    | esframe(1)
20    | eoln(1)
21    | coblow(1)
24    | eof(2)
25    | euflow(2)
26    | esline(2)
27    | esframe(2)
28    | eoln(2)
29    | coblow(2)

## `SCALER_DISPDITHER`

based on https://www.raspberrypi.org/forums/viewtopic.php?f=41&t=45290&p=363475#p363475 and a recent start.elf decompile

bits | usage
---|---
0:1   | channel 0 type
2:3   | channel 0 depth
8:9   | channel 1 type
10:11 | channel 1 depth
16:17 | channel 2 type
18:19 | channel 2 depth
30    | unknown

from the disassembly, it appears to just be a 4bit field containing the `dither_type` and `dither_depth`, repeated 3 times, once per channel

## `SCALER_DISPEOLN`

3 groups of 9 bits, one group per hvs channel

bits | usage
---|---
30 | enable something?

## `SCALER_DISPBKGNDn`

bits | usage
---|---
0:23 | background color
24 | fill with background color, costs more clock cycles
25:28 | test mode
29 | respect the gamme bits in `SCALER_DISPGAMDAT`?
30 | interlace
31 | autohs 

## `SCALER_DISPGAMADR`
bits | usage
---|---
30 | enable gamma sram
31 | autoincrement

## `SCALER_DISPGAMDAT`

i think `SCALER_DISPGAMADR` sets the addr into the gamma ram, and if it increments on each write to `SCALER_DISPGAMDAT`

each hvs channel starts 0x300 bytes from the last?


# display list information

on VC4, the display list starts at base + 0x2000, on VC6 its at base + 0x4000

the displaylist is 0x4000 bytes long

a display list is a series of planes in the dlist memory, followed by a `SCALER_CTL0_END` flag

for example: `[CTL(valid,...), POS0, POS2, POS3, POINT0, POINTCTX, PITCH0, CTL(END)]` would be a list containing just one plane

## VC4 Control Word:
bits | usage
---- |-----
0:3  | pixel format
4    | unity scaling
5:7  | SCL0
8:10 | SCL1
11:12| rgb expand
13:14| pixel order
15   | vflip
16   | hflip
17:18| key mode
19   | alpha mask
20:21| tiling mode
24:29| number of words in this plane description
30   | marks this as a valid control word
31   | signals the end of a display list (must be its own control word, after a another plane)

## VC6 Control Word:
bits | usage
---- | -----
0:4  | pixel format
5:7  | SCL0
8:10 | SCL1
11   | rgb expand
12   | alpha expand
13:14| pixel order
15   | unity scaling
20:21| tiling mode
30   | marks this as a valid control word
31   | signals the end of a display list (must be its own control word, after a another plane)

## VC6 Control Word 2:
bits | usage
-----|---
4:15 | alpha
16   | gamma
17:18| `MAP_SEL`
25   | `ALPHA_LOC`
28   | `ALPHA_MIX`
29   | `ALPHA_PREMULT`
30:31| alpha mode

## VC4 Position Word 0:
bits | usage
-----|------
0:11 | x
12:23| y
24:31| fixed alpha

## VC6 Position Word 0:
bits | usage
-----|-------
0:13 | x
15   | hflip
16:27| y
31   | yflip

## VC4 Position Word 1:
only present when not using unity scaling

bits | usage
-----|----
0:11 | scale width
16:27| scale height

## VC6 Position Word 1:
only present when not using unity scaling

bits | usage
-----|----
0:12 | scale width
16:28| scale height

## VC4 Position Word 2:
bits | usage
-----|---
0:11 | width
16:27| height
28   | alpha mix
29   | alpha pre-multiply
30:31| alpha mode

## VC6 Position Word 2:
bits | usage
-----|---
0:12 | width
16:28| height

## Position Word 3:
the HVS will store some state in this slot when running, just fill it with any 32bit value when generating the display list

## Pointer Word 0/1/2:
the address for each plane of image data
RGB based formats only use Pointer Word 0, while YUV formats use 3

## Pointer Context Word 0/1/2:
more internal state for the HVS, one per Pointer Word used

## Pitch Word 0/1/2:
the pitch for each plane

## more info:
Position Word 0 mainly describes where on the screen to render a given image

Position Word 1 describes the destination width/height, when scaling

Position word 2 mainly describes the source width/height, and if unity, that is also the destination size

## example plane configs:

VC4 RGB unity scaling:
Control Word, Position Word 0, Position Word 2, Position Word 3, Pointer Word 0, Pitch Word 0

VC6 RGB unity scaling:
Control Word, Position Word 0, Control Word 2, Position Word 2, Position Word 3, Pointer Word 0, Pitch Word 0

# misc notes
`hvs_priority` in config.txt sets its axi priority?
https://www.raspberrypi.org/forums/viewtopic.php?f=38&t=79330&p=584389&hilit=hvs_priority#p584389
