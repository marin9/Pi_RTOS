#pragma once
#include "types.h"

#define TASK_RUNNING	0
#define TASK_WAITING	1

#define SEM_BIN			0
#define SEM_COUNT		1

#define ERR_ARGS	1
#define ERR_NOMEM	2
#define ERR_NOOBJ	3


// Interrupts
void interrupt_handler();

// Memory
void memory_init(void *addr);
void* memory_alloc(uint size);
void memory_free(void *p);

// Time
void time_init();
void time_tick();
uint time_get();
void time_wait(uint ms);

// Task
void start_sched();
void task_sched();
void task_yield();
int task_init(void (*main)());
int task_create(void *func, void *arg);
int task_exit(uint id);
int task_wait(queue_t *q);
int task_wakeup(queue_t *q);
int task_wakeup_all(queue_t *q);
int task_count();
int task_self();
int task_join(uint id);

// Semaphore
int sem_init(sem_t *s, int val, int type);
int	sem_wait(sem_t *s);
int	sem_post(sem_t *s);
int	sem_trywait(sem_t *s);
