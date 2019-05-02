#pragma once
#include "types.h"

// Interrupts
void interrupt_handler();

// Memory
void memory_init(void *addr, uint bsize, uint n);
void* memory_alloc(uint size);
void memory_free(void *p);

// Time
void time_init();
void time_tick();
void time_set(time_t *newtime);
void time_get(time_t *current);
