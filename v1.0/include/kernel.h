#pragma once
#include "types.h"

#define TASK_RUNNING    0
#define TASK_SLEEPING   1
#define TASK_BLOCKED	2

#define SEM_BIN		0
#define SEM_COUNT	1


// Interrupts
void interrupt_handler();

// Memory
void memory_init(void *addr, uint bsize, uint n);
void* memory_alloc(uint size);
void memory_free(void *p);

// Time
void time_init();
void time_tick();
uint time_get();

// Task
void task_scheduler();
void task_run_next();
void task_init(void (*main)());
int task_create(void *func, void *arg, int prio);
int task_sleep(int tid, int ms);
int task_wakeup(int tid);
int task_status(int tid);
int task_exit(int tid);
int task_count();
int task_self();

// Semaphore
void sem_init();
int	sem_create(int val, int type);
int	sem_delete(int sid);
int	sem_wait(int sid);
int	sem_trywait(int sid);
int	sem_post(int sid);
