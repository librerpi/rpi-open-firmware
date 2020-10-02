there are 3 main stages to the video output pipeline in the rpi

first stage is the [HVS](https://blog.benjdoherty.com/2019/05/21/Exploring-Hardware-Compositing-With-the-Raspberry-Pi/), it will take a display list, convert it into scanlines, and feed them into a FIFO

second stage is the [pixelvalve](pixelvalve.md), it takes all of the [video timing information](https://wiki.osdev.org/Video_Signals_And_Timing), generates the hsync/vsync pulses, and throttles the flow of pixels from the HVS FIFO to the output PHY

the third stage is the various output PHY's
* [hdmi](hdmi.md)
* VEC (ntsc/pal video)
* [DPI](https://www.raspberrypi.org/documentation/hardware/raspberrypi/dpi/README.md)
* SMI
* [DSI](https://www.raspberrypi.org/documentation/hardware/display/README.md)

all pi models have 3 HVS channels, and can drive a total of 3 screens at once (but the official firmware only supports a max of 2)

VC4 pi's have 3 pixelvalves:
* PV0 can drive DSI0 or DPI
* PV1 can drive DSI1 or SMI
* PV2 can drive HDMI or VEC

VC6 pi's have 5 pixelvalves:
* PV0 can drive DSI0/DPI
* PV1 can drive DSI1/SMI
* PV2 can drive HDMI0
* PV3 can drive VEC
* PV4 can drive HDMI1

on VC4, each PV is hard-wired to a certain HVS channel
but on VC6, each PV has a mux to select between any HVS channel(todo, confirm that), but you cant share a channel between 2 PV's


```
+-----+  +----------+  +----+  +-----+
| HVS |->| HVS FIFO |->| PV |->| PHY |
+-----+  +----------+  +----+  +-----+
```

all 3 HVS channels share some FIFO ram, and the `SCALER_DISPBASE[0-2]` registers control which region of the ram each will be using

some example code:
```
  hvs_channels[2].dispbase = BASE_BASE(0)      | BASE_TOP(0xf00);
  hvs_channels[1].dispbase = BASE_BASE(0xf10)  | BASE_TOP(0x4b00);
  hvs_channels[0].dispbase = BASE_BASE(0x4b10) | BASE_TOP(0x7700);
```
under normal conditions, those ranges will have already been configured by the firmware, so the only reason to look at them is to know the latency between the HVS and the PV

https://github.com/raspberrypi/linux/blob/rpi-4.19.y/drivers/gpu/drm/vc4/vc4_crtc.c#L1136-L1150

https://github.com/raspberrypi/linux/blob/rpi-4.19.y/drivers/gpu/drm/vc4/vc4_crtc.c#L173-L196

the lower 12 bits of `SCALER_DISPSTAT[0-2]` say what scanline the HVS is currently on, but based on width and fifo depth, the PV+PHY will be lagging behind a certain number of scanlines
