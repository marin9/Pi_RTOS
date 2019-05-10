#pragma once
#include "types.h"
#include "core.h"

void* Malloc(uint size);
void Mfree(void *p);

uint Time();
void Sleep(uint ms);

int Task_create(void *func, void *arg);
int Task_wait(queue_t *q);
int Task_wakeup(queue_t *q);
int Task_wakeup_all(queue_t *q);
int Task_exit(uint id);
int Task_count();
int Task_self();
void Task_yield();

int Sem_init(sem_t *s, int val, int type);
int	Sem_wait(sem_t *s);
int	Sem_post(sem_t *s);
int	Sem_trywait(sem_t *s);
