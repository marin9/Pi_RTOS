#pragma once
#include "types.h"

void irq_init();
void irq_register(uint irq, void (*h)());
void irq_unregister(uint irq);
