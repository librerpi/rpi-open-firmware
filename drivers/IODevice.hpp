#pragma once

#include <runtime.h>
#include <hardware.h>

enum IODeviceState {
	kIODeviceUninitialized,
	kIODeviceStarted,
	kIODeviceStopped
};

struct IODevice {
	const char* driverName;
	IODeviceState deviceState;
	uint32_t tag;
	
	IODevice();

	void driverLog(const char* fnName, const char* fmt, ...);
	void registerDriver();

	void inline setName(const char* name) {
		driverName = name;
	}

	void inline setTag(uint32_t tag) {
		this->tag = tag;
	}

	virtual void start();
	virtual void stop();
	virtual void init();

	void _afterInit();
	void _beforeInit();

	virtual inline IODeviceState getDeviceState() {
		return deviceState;
	}

	static IODevice* findByTag(uint32_t tag);
};

#define IODriverLog(fmt, ...) driverLog(__FUNCTION__, fmt, ##__VA_ARGS__)
#define IODriverCreateSingletonInstance(clazz) static clazz __IODriver_static_ ## clazz {}
#define IODriverConstructor(clazz) inline clazz() { _beforeInit(); driverName = #clazz; init(); _afterInit(); }
