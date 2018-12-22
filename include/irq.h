#pragma once

void irq_init();
void irq_register(int irq_num, void (*handler)(), void (*clearer)());
void irq_unregister(int irq_num);
