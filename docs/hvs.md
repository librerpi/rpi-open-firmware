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
