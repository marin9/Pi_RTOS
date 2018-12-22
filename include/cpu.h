#pragma once
#include "task.h"

void cpu_init();
void cpu_suspend();
void cpu_halt();
void context_switch(task_t *old, task_t *new);
int enable_interrupts();
int disable_interrupts();
