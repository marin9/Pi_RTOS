#pragma once
#include "types.h"

// Interrupts
void interrupt_init();
void interrupt_enable();
void interrupt_disable();
void interrupt_handler();

// Memory
void memory_init(void *addr, uint bsize, uint n);
void* memory_alloc(uint size);
void memory_free(void *p);

// Time
void time_init();
