#include <drivers/IODevice.hpp>

static IODevice* g_DeviceRegistry[32] = { 0 };
static unsigned int g_NewDriverIndex = 0;

IODevice::IODevice() {

}

void IODevice::_beforeInit() {
	tag = 0;
	deviceState = kIODeviceUninitialized;
}

void IODevice::_afterInit() {
	assert(driverName);
	if (tag)
		registerDriver();
}

void IODevice::registerDriver() {
	g_DeviceRegistry[g_NewDriverIndex++] = this;
	IODriverLog("driver registered on platform IO plane");
}

void IODevice::driverLog(const char* fnName, const char* fmt, ...)
{
	va_list va;
        uint32_t clock_lo = ST_CLO;

	printf("0x%08x %s::%s(): ", clock_lo, driverName, fnName);
	va_start(va, fmt);
	vprintf(fmt, va);
	va_end(va);
	printf("\n");
}

IODevice* IODevice::findByTag(uint32_t tag) {
	for (int i = 0; i < (sizeof(g_DeviceRegistry) / sizeof(void*)); i++) {
		IODevice* dev = g_DeviceRegistry[i];
		if (dev && dev->tag == tag) {
			return dev;
		}
	}

	return nullptr;
}

void IODevice::start() {
	deviceState = kIODeviceStarted;
}

void IODevice::stop() {
	deviceState = kIODeviceStopped;
}

void IODevice::init() {
	panic("IODevice objects have to override init()");
}


