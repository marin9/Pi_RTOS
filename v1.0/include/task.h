#pragma once
#include "list.h"
#include "time.h"

#define TASK_STACK_SIZE  4096

#define TASK_TERMINATED 0
#define TASK_RUNNING    1
#define TASK_SLEEPING   2
#define TASK_BLOCKED    3


typedef struct{
  unsigned int r0;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r4;
  unsigned int r5;
  unsigned int r6;
  unsigned int r7;
  unsigned int r8;
  unsigned int r9;
  unsigned int r10;
  unsigned int r11;
  unsigned int cpsr;
  unsigned int sp;
  unsigned int lr;
} context_t;

typedef struct{
  context_t *context;
  void *stack;
  int tid;
  int prio;
  int status;
  list_t *queue;
  time_t sleep;
  node_t node;
  node_t nodeall;
} task_t;


int comp_time(void *o1, void *o2);
int comp_tid(void *o1, void *o2);
int comp_prio(void *o1, void *o2);

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