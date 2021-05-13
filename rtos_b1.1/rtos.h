#pragma once

#define TICK_TIME		1000
#define THREAD_COUNT	16
#define STACK_SIZE		4096
#define PRIO_COUNT		256
#define SEM_COUNT		8
#define MBOX_COUNT		2
#define MBOX_SIZE		4

#define ERR_OK			0
#define ERR_ISR			1
#define ERR_NORES		2
#define ERR_PARAMS		3
#define ERR_NOBLOCK		4
#define ERR_OVERFLW		5
#define ERR_CONFIG		6

typedef unsigned int uint;
typedef void (*handler)(void *arg);
typedef struct thread thread;
typedef struct queue queue;
typedef struct mbox mbox;
typedef struct sem sem;


struct thread {
	uint *sp;
	uint id;
	uint prio;
	uint param;
	uint status;
	struct queue *q;
	struct thread *next;
};

struct queue {
	thread *first;
	thread *last;
};

struct sem {
	uint max;
	uint value;
	queue waitq;
};

struct mbox {
	uint data[MBOX_SIZE];
	uint count;
	uint index_in;
	uint index_out;
	queue wait_tx;
	queue wait_rx;
};

int rtos_init();
void rtos_start();
uint rtos_ticks();

int thread_create(handler func, void *arg, uint prio);
int thread_sleep(uint ticks);
int thread_term(uint id);
int thread_getid();

int sem_init(uint id, uint value, uint max);
int sem_wait(uint id);
int sem_trywait(uint id);
int sem_signal(uint id);

int mbox_init(uint id);
int mbox_send(uint id, int data, uint wait);
int mbox_recv(uint id, int *data, uint wait);
