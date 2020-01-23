/*
 * VideoCore4_Drivers
 * Copyright (c) 2017 Authors of rpi-open-firmware
 *
 * USB PHY initialization driver.
 */

#include <drivers/IODevice.hpp>
#include <drivers/BCM2708PowerManagement.hpp>
#include <drivers/BCM2708Gpio.hpp>
#include <hang_cpu.h>

#define FLAG_BUSY (1 << 31)

struct BCM2708UsbPhy : IODevice {
	IODriverConstructor(BCM2708UsbPhy);

	void wait() {
		while(USB_GMDIOCSR & FLAG_BUSY);
	}

	void write_bare(int reg, uint16_t value, int type) {
		reg &= 0x1F;

		/* precede MDIO access */
		USB_GMDIOGEN = 0xFFFFFFFF;
		wait();

		/* write the actual value, with flags */
		USB_GMDIOGEN = type | (reg << 18) | value;
		wait();

		/* dummy write due to errata; see BCM2835 peripheral manual */
		USB_GMDIOGEN = 0;
		wait();
	}

	uint16_t usb_read(int reg) {
		write_bare(reg, 0, 0x60020000);
		return USB_MDIO_CSR & 0xFFFFF;
	}

	virtual void usb_write(int reg, uint16_t value) {
		write_bare(reg, value, 0x50020000);
	}

	virtual void start() override {
		IODriverLog("starting ...");

		/* enable clock */
		A2W_XOSC_CTRL |= A2W_PASSWORD | A2W_XOSC_CTRL_USBEN_SET;
		while(!(A2W_XOSC_CTRL & A2W_XOSC_CTRL_USBOK_SET));

		CM_TDCLKEN |= CM_PASSWORD | CM_TDCLKEN_USBDFT_SET;

		/* the LAN_RUN pin is GPIO6 according to the schematic */
		/* edit: it's different between models.
		 * see https://github.com/raspberrypi/firmware/blob/master/extra/dt-blob.dts#L711
		 * e.g. on the RPi 2B it's pin 31
		 */


                BCM2708Gpio *gpio = static_cast<BCM2708Gpio*>(IODevice::findByTag(GPIO_TAG));
                gpio->setFunction(31, kBCM2708PinmuxOut);

		udelay(300);
		GP_CLR0 = (1 << 31);
		udelay(300);
		GP_SET0 = (1 << 31);
		udelay(300);

		IODriverLog("LAN reset");

		USB_GMDIOCSR = (1 << 18);

		usb_write(0x15, 272 /* devmode ? 4369 : 272*/);
		usb_write(0x19, 0x4);
		usb_write(0x18, 0x10);
		usb_write(0x1D, 0x4);
		usb_write(0x17, 5682);

		while((usb_read(0x1B) & (1 << 7)) != 0);

		USB_GVBUSDRV &= ~(1 << 7);

		usb_write(0x1E, 0x8000);

		usb_write(0x1D, 0x5000);
		usb_write(0x19, 0xC004);
		usb_write(0x20, 0x1C2F);
		usb_write(0x22, 0x0100);
		usb_write(0x24, 0x0010);
		usb_write(0x19, 0x0004);

		USB_GVBUSDRV = USB_GVBUSDRV & 0xFFF0FFFF | 0xD0000;
		udelay(300);
		mmio_write32(0x7E980400 + 3084, 0x20402700);
		udelay(300);
		mmio_write32(0x7E980400, 1);
		udelay(300);
		mmio_write32(0x7E980404, 0xBB80);
		udelay(300);

		IODriverLog("started");

		IODevice::start();
	}

	virtual void init() override {
		setName("BCM2708UsbPhy");
		setTag('USBP');
	}
};

IODriverCreateSingletonInstance(BCM2708UsbPhy);

