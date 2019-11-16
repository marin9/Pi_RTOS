#pragma once
#include "types.h"

#define TASK_COUNT		8
#define PRIO_COUNT		4
#define SEM_COUNT		4
#define TICK_TIME		1000
#define STACK_SIZE		(2*1024)


void task_init();
void task_sched_start();

void task_start(func fn, uint pr);
void task_sleep(uint ticks);
void task_exit();
void sem_init(uint id, uint v);
void sem_take(uint id);
void sem_give(uint id);
int sem_try(uint id);
