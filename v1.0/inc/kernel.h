#pragma once
#include "types.h"

#define TASK_RUNNING	0
#define TASK_WAITING	1

#define SEM_BIN			0
#define SEM_COUNT		1


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
void time_wait(uint ms);

// Task
void task_sched();
void start_sched();
int task_init(void (*main)());
int task_create(void *func, void *arg);
int task_exit(uint id);
int task_wait(queue_t *q);
int task_wakeup(queue_t *q);
int task_wakeup_all(queue_t *q);
int task_count();
int task_self();
void task_yield();

// Semaphore
void sem_init();
//int	sem_create(int val, int type);
//int	sem_delete(int sid);
//int	sem_wait(int sid);
//int	sem_post(int sid);
//int	sem_trywait(int sid);

//TODO add pipe or msgq
