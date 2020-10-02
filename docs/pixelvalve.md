register  | offset
--- | ---
C         | 0x0
VC        | 0x4
`VSYNCD_EVEN` | 0x8
HORZA     | 0xc
HORZB     | 0x10
VERTA     | 0x14
VERTB     | 0x18
`VERTA_EVEN` | 0x1c
`VERTB_EVEN` | 0x20
`INTEN`   | 0x24
`INTSTAT` | 0x28
`STAT`    | 0x2c
`DSI_HACT_ACT` | 0x30

## PV C register
bits | usage
-----|-----
0    | enable
1    | fifo clear
2:3  | clock select
4:5  | pixel rep
12   | wait hstart
13   | trigger underflow
14   | clear at start
15:20| fifo full level bits 0:5
21:23| format
24   | unknown
25:26| fifo full level bits 6:7 (VC6 only)

## PV VC register
bits | usage
---|---
0  | video enable
1  | continous
2  | command
3  | dsi
4  | interlace
5  | odd first
6  | odd delay

## PV HORZA
bits | usage
---|---
0:15 | hsync
16:31 | h backporch

## PV HORZB
bits | usage
---|---
0:15 | hactive
16:31 | h frontporch

## PV VERTA
bits | usage
---|---
0:15 | vsync
16:31 | v backporch

## PV VERTB
bits | usage
---|---
0:15 | vactive
16:31 | v frontporch

## PV INTEN
interrupt enable
bits | usage
---|---
0 | hsync start
1 | h backporch start
2 | hactive start
3 | h frontporch start
4 | vsync start
5 | v backporch start
6 | vactive start
7 | v frontporch start
8 | v frontporch end
9 | video idle
## PV INTSTAT
interrupt status, same bits as INTEN

# overview

each scanline has HFP clocks worth of front porch, HSYNC clocks worth of hsync, HBP worth of back porch, and then HACTIVE clocks worth of pixel data

on VC6, some PV's operate in DDR mode, and generate 2 pixels per clock, so all horizoncal timings need to be halved when put into the registers

one field has VFP scanlines of frontporch, VSYNC scanlines of sync, VBP scanlines of backporch, and then VACTIVE scanlines of actual image data

when interlacing is on, it will alternate between the normal VERTA/VERTB and `VERTA_EVEN`/`VERTB_EVEN`
