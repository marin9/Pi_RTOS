#pragma once
#include "types.h"

void* Malloc(uint size);
void Mfree(void *p);

int Task_create(void *func, void *arg);
int Task_exit(uint id);
int Task_count();
int Task_self();
void Task_yield();
