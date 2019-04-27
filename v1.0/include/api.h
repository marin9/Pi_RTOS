#pragma once
#include "time.h"
#include "sem.h"

// Console
int Gets(char *s, int n);
int Printf(char *format, ...);

// Memory
void* Malloc(int size);
void Free(void* p);

// Time
void TimeSet(time_t *newtime);
void TimeGet(time_t *current);

// Task
int TaskCreate(void *func, void *arg, int prio);
int TaskSleep(int tid, int ms);
int TaskWakeup(int tid);
int TaskStatus(int tid);
int TaskExit(int tid);
int TaskCount();
int TaskSelf();

// Semaphore
int	SemCreate(int val, int type);
int	SemDelete(int sid);
int	SemWait(int sid);
int	SemTryWait(int sid);
int	SemPost(int sid);