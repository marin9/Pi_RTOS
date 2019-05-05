#pragma once

// Basic
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

// Lib
typedef struct node_t{
	void *object;
	struct node_t* next;
	struct node_t* prev;
} node_t;

typedef struct queue_t{
	int count;
	node_t *first;
	node_t *last;
} queue_t;

// Processor
typedef struct{
  uint r0;
  uint r1;
  uint r2;
  uint r3;
  uint r4;
  uint r5;
  uint r6;
  uint r7;
  uint r8;
  uint r9;
  uint r10;
  uint r11;
  uint cpsr;
  uint sp;
  uint lr;
} context_t;

// Task
typedef struct{
  context_t *context;
  void *stack;
  uint id;
  uint status;
  uint sleep;
  node_t nd;
  node_t ndall;
} task_t;

// Semaphore
typedef struct{
  uint id;
  uint type;
  uint value;
  queue_t wait_tasks;
  node_t nd;
} sem_t;
