#pragma once
#include "types.h"

void interrupt_init();
void interrupt_register(uint irq, void (*h)());
void interrupt_unregister(uint irq);
