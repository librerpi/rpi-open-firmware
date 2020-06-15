#pragma once

struct peripherals {
  int fd;
  void *peripherals_start;
  unsigned int physical_start;
};

void open_peripherals(struct peripherals &handle);
