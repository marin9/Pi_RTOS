#pragma once

#define TICK_TIME		1000
#define PREEMPTIVE		0
#define THREAD_COUNT	16
#define STACK_SIZE		4096
#define SEM_COUNT		8
#define MBOX_COUNT		2
#define MBOX_SIZE		4

#define ERR_OK			0
#define ERR_ISR			1
#define ERR_NORES		2
#define ERR_PARAMS		3
#define ERR_NOBLOCK		4

typedef unsigned int uint;
typedef void (*handler)(void *arg);
typedef struct thread thread;
typedef struct queue queue;
typedef struct mbox mbox;
typedef struct sem sem;


struct thread {
	uint *sp;
	uint status;
	uint sleep;
	struct thread *next;
};

struct queue {
	thread *first;
	thread *last;
};

struct sem {
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

void rtos_init();
void rtos_start();
uint rtos_ticks();

int thread_create(handler func, void *arg);
int thread_sleep(uint ticks);
int thread_term();

int sem_init(uint id, uint value);
int sem_wait(uint id);
int sem_trywait(uint id);
int sem_signal(uint id);

int mbox_init(uint id);
int mbox_send(uint id, int data, uint wait);
int mbox_recv(uint id, int *data, uint wait);
