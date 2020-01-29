#pragma once

struct BCM2708ArmControl : IODevice {
  BCM2708ArmControl();

  PowerManagementDomain* pmDomain;

  static inline void bridgeWriteBresp(uint8_t bits);
  void bridgeCycleBresp();
  void bridgeStart(bool cycleBrespBits);
  void setupClock();
  void setupOtherClocks();
  void patchFirmwareData();
  void loadInitialCode();
  void mapBusToArm(uint32_t busAddr, uint32_t armAddr);
  void printregs();
  void bzero2(void *addr, size_t len);
  virtual void start() override;
  virtual void init() override;
};

