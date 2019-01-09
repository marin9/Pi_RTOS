#include "api.h"
#include "err.h"
#include "string.h"
#include "stdio.h"
#include "mem.h"
#include "time.h"
#include "task.h"
#include "sem.h"


// Console
int Getc(){
  return getc();
}

int Putc(int c){
  return putc(c);
}

int Printf(char *format, ...){
  char buffer[256];
  vssprintf(buffer, 256, &format);
  return puts(buffer);
}


// Memory
void* Malloc(int size){
  void *p;
  disable_interrupts();
  p=mem_alloc(size);
  enable_interrupts();
  return p;
}

void Free(void* p){
  disable_interrupts();
  mem_free(p);
  enable_interrupts();
}


// Time
void TimeSet(time_t *newtime){
  disable_interrupts();
  time_set(newtime);
  enable_interrupts();
}

void TimeGet(time_t *current){
  disable_interrupts();
  time_get(current);
  enable_interrupts();
}


// Task
int TaskCreate(void *func, void *arg, int prio){
  int val;
  disable_interrupts();
  val=task_create(func, arg, prio);
  enable_interrupts();
  return val;
}

int TaskSleep(int tid, int ms){
  int val;
  disable_interrupts();
  val=task_sleep(tid, ms);
  enable_interrupts();
  return val;
}

int TaskWakeup(int tid){
  int val;
  disable_interrupts();
  val=task_wakeup(tid);
  enable_interrupts();
  return val;
}

int TaskStatus(int tid){
  int val;
  disable_interrupts();
  val=task_status(tid);
  enable_interrupts();
  return val;
}

int TaskExit(int tid){
  int val;
  disable_interrupts();
  val=task_exit(tid);
  enable_interrupts();
  return val;
}

int TaskCount(){
  int val;
  disable_interrupts();
  val=task_count();
  enable_interrupts();
  return val;
}

int TaskSelf(){
  int val;
  disable_interrupts();
  val=task_self();
  enable_interrupts();
  return val;
}


// Semaphore
int SemCreate(int val, int type){
  int sid;
  disable_interrupts();
  sid=sem_create(val, type);
  enable_interrupts();
  return sid;
}

int SemDelete(int sid){
  int v;
  disable_interrupts();
  v=sem_delete(sid);
  enable_interrupts();
  return v;
}

int SemWait(int sid){
  int v;
  disable_interrupts();
  v=sem_wait(sid);
  enable_interrupts();
  return v;
}

int SemTryWait(int sid){
  int v;
  disable_interrupts();
  v=sem_trywait(sid);
  enable_interrupts();
  return v;
}

int SemPost(int sid){
  int v;
  disable_interrupts();
  v=sem_post(sid);
  enable_interrupts();
  return v;
}

