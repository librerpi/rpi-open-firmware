#pragma once

void set_interrupt(int intno, bool enable, int core);
void setup_irq_handlers(void);
typedef void (*irqType)();

extern irqType vectorTable[];
