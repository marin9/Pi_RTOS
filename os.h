#pragma once

#define STACK_SIZE		4096
#define TICK_TIME		1000
#define TASK_COUNT		16
#define PRIO_COUNT		4

#define ERR_OK			0
#define ERR_NORES		1
#define ERR_PARAMS		2
#define ERR_NOBLOCK		3
#define ERR_OVERFLOW	4

#define CPU_LED			47

typedef void (*func)();
typedef unsigned int uint;

typedef struct {
	uint r0, r1, r2, r3, r4, r5, r6, r7;
	uint r8, r9, r10, r11, cpsr, lr, pc;
} context_t;

typedef struct task_t {
	uint *sp;
	uint prio;
	uint param;
	uint status;
	struct task_t *next;
} task_t;

typedef struct {
	task_t *first;
	task_t *last;
} queue_t;

typedef struct {
	uint max;
	uint value;
	queue_t waitq;
} sem_t;

typedef struct {
	int *buff;
	uint size, count;
	uint x_in, x_out;
	queue_t wait_in;
	queue_t wait_out;
} mbox_t;


void os_init();
void os_start();
uint os_ticks();

int task_create(func fn, void *arg, uint prio);
void task_sleep(uint ticks);
void task_term();

int sem_init(sem_t *s, uint val, uint max);
int sem_wait(sem_t *s, uint wait);
int sem_signal(sem_t *s);

int mbox_init(mbox_t *mb, int *buff, uint size);
int mbox_send(mbox_t *mb, int data, uint wait);
int mbox_recv(mbox_t *mb, int *data, uint wait);
