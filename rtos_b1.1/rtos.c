#include "rtos.h"
#include "arm.h"
#include "rpi.h"

#define THREAD_UNUSED		0
#define THREAD_READY		1
#define THREAD_SLEEP		2
#define THREAD_BLOCKED		3

#define SYS_ENTRY()		irq_disable()
#define SYS_EXIT(cpsr)	irq_restore(cpsr)

static uint sys_time;
static uint sched_run;
thread *active_thread;
static queue ready_queue;
static queue sleep_queue;
static sem semaphore[SEM_COUNT];
static mbox mailbox[MBOX_COUNT];
static thread threads[THREAD_COUNT];
static char stack[THREAD_COUNT][STACK_SIZE] __attribute__((aligned(8)));


// Queue API -------------------------------------------------------------------

static void queue_init(queue *q) {
	q->first = 0;
	q->last = 0;
}

static void queue_push(queue *q, thread *t) {
	t->next = 0;
	t->q = q;

	if (!q->first)
		q->first = t;
	else
		q->last->next = t;

	q->last = t;
}

static void queue_insert(queue *q, thread *t) {
	thread *it;
	thread *it2;

	t->q = q;
	// list is empty
	if (q->first == 0) {
		queue_push(q, t);
		return;
	}
	// if new node should be first
	it = q->first;
	if (it->param > t->param) {
		t->next = it;
		q->first = t;
		return;
	}
	// search position
	while (it->next && (t->param > it->next->param))
		it = it->next;

	it2 = it->next;
	// new node should be the last
	if (it2 == 0) {
		queue_push(q, t);
		return;
	}
	// insert betwen
	it->next = t;
	t->next = it2;
}

static thread* queue_pop(queue *q) {
	thread *tmp = q->first;
	if (!tmp)
		return 0;

	q->first = tmp->next;
	if (!q->first)
		q->last = 0;

	tmp->q = 0;
	return tmp;
}

void queue_remove(thread *n) {
	thread *it;
	queue *q;

	q = n->q;

	if (q->first == n) {
		queue_pop(q);
		return;
	}

	it = q->first;
	while (it->next != n)
		it = it->next;

	it->next = n->next;

	if (q->last == n)
		q->last = it;
}

static thread* queue_peek(queue *q) {
	return q->first;
}

// Scheduler -------------------------------------------------------------------

void rtos_sched() {
	if (active_thread && active_thread->status == THREAD_READY) {
		active_thread->param = active_thread->prio;
		queue_insert(&ready_queue, active_thread);
	}

	active_thread = queue_pop(&ready_queue);
}

void rtos_tick() {
	thread *tmp;

	++sys_time;
	timer_set(TICK_TIME);

	tmp = queue_peek(&sleep_queue);
	while (tmp && (sys_time >= tmp->param)) {
		tmp = queue_pop(&sleep_queue);
		tmp->status = THREAD_READY;
		tmp->param = tmp->prio;
		queue_insert(&ready_queue, tmp);
		tmp = queue_peek(&sleep_queue);
	}

	tmp = queue_peek(&ready_queue);
	if (tmp->prio < active_thread->prio)
		rtos_sched();
}

void thread_exit() {
	SYS_ENTRY();
	active_thread->status = THREAD_UNUSED;
	SVC();
}

static void idle() {
	while (1) SVC();
}


// OS API ----------------------------------------------------------------------

int rtos_init() {
	int i;

	if (TICK_TIME < 1)
		return -ERR_CONFIG;

	if (THREAD_COUNT < 2)
		return -ERR_CONFIG;

	if (STACK_SIZE < 256)
		return -ERR_CONFIG;

	if (PRIO_COUNT < 1)
		return -ERR_CONFIG;

	if (SEM_COUNT < 0)
		return -ERR_CONFIG;

	if (MBOX_COUNT < 0)
		return -ERR_CONFIG;

	if (MBOX_SIZE < 1)
		return -ERR_CONFIG;

	sys_time = 0;
	sched_run = 0;
	active_thread = 0;

	for (i = 0; i < THREAD_COUNT; ++i)
		threads[i].status = THREAD_UNUSED;

	queue_init(&ready_queue);
	queue_init(&sleep_queue);
	thread_create(idle, 0, PRIO_COUNT - 1);
	return ERR_OK;
}

void rtos_start() {
	if (sched_run)
		return;

	sched_run = 1;
	timer_set(TICK_TIME);
	pic_enable(IRQ_TMR);
	SVC();
}

uint rtos_ticks() {
	return sys_time;
}


int thread_create(handler func, void *arg, uint prio) {
	int i;

	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	uint s = SYS_ENTRY();

	for (i = 0; i < THREAD_COUNT; ++i) {
		if (threads[i].status == THREAD_UNUSED)
			break;

		if (i == THREAD_COUNT - 1) {
			SYS_EXIT(s);
			return -ERR_NORES;
		}
	}

	threads[i].id = i;
	threads[i].prio = prio;
	threads[i].sp = arm_create_context(stack[i] + STACK_SIZE,
										func, arg, thread_exit);
	threads[i].status = THREAD_READY;
	threads[i].param = prio;
	queue_insert(&ready_queue, &threads[i]);

	if (sched_run && active_thread && prio > active_thread->prio)
		SVC();

	SYS_EXIT(s);
	return i;
}

int thread_sleep(uint ticks) {
	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	uint s = SYS_ENTRY();
	if (ticks) {
		active_thread->param = ticks + sys_time;
		active_thread->status = THREAD_SLEEP;
		queue_insert(&sleep_queue, active_thread);
	}

	SVC();
	SYS_EXIT(s);
	return ERR_OK;
}

int thread_term(uint id) {
	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	if (id >= THREAD_COUNT || id == 0)
		return -ERR_PARAMS;

	uint s = SYS_ENTRY();
	if (id == active_thread->id)
		thread_exit();

	threads[id].status = THREAD_UNUSED;
	queue_remove(&threads[id]);

	SYS_EXIT(s);
	return ERR_OK;
}

int thread_getid() {
	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	return active_thread->id;
}


int sem_init(uint id, uint value, uint max) {
	if (id >= SEM_COUNT)
		return -ERR_PARAMS;

	if (value > max)
		return -ERR_PARAMS;

	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	semaphore[id].max = max;
	semaphore[id].value = value;
	queue_init(&semaphore[id].waitq);
	return ERR_OK;
}

int sem_wait(uint id) {
	if (id >= SEM_COUNT)
		return -ERR_PARAMS;

	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	uint s = SYS_ENTRY();

	if (semaphore[id].value > 0) {
		semaphore[id].value -= 1;
	} else {
		active_thread->status = THREAD_BLOCKED;
		queue_push(&semaphore[id].waitq, active_thread);
		SVC();
	}
	SYS_EXIT(s);
	return ERR_OK;
}

int sem_trywait(uint id) {
	if (id >= SEM_COUNT)
		return -ERR_PARAMS;

	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	uint s = SYS_ENTRY();

	if (semaphore[id].value > 0) {
		semaphore[id].value -= 1;
		SYS_EXIT(s);
		return ERR_OK;
	} else {
		SYS_EXIT(s);
		return ERR_NOBLOCK;
	}
}

int sem_signal(uint id) {
	thread *tmp;

	if (id >= SEM_COUNT)
		return -ERR_PARAMS;

	uint s = SYS_ENTRY();

	tmp = queue_pop(&semaphore[id].waitq);
	if (tmp) {
		tmp->status = THREAD_READY;
		tmp->param = tmp->prio;
		queue_insert(&ready_queue, tmp);
	} else if (semaphore[id].value == semaphore[id].max) {
		SYS_EXIT(s);
		return -ERR_OVERFLW;
	} else {
		semaphore[id].value += 1;
	}
	SYS_EXIT(s);
	return ERR_OK;
}

int mbox_init(uint id) {
	if (id >= MBOX_COUNT)
		return -ERR_PARAMS;

	if (get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	mailbox[id].count = 0;
	mailbox[id].index_in = 0;
	mailbox[id].index_out = 0;
	queue_init(&mailbox[id].wait_tx);
	queue_init(&mailbox[id].wait_rx);
	return ERR_OK;
}

int mbox_send(uint id, int data, uint wait) {
	if (id >= MBOX_COUNT)
		return -ERR_PARAMS;

	if (wait && get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	thread *tmp;
	uint in;
	uint s = SYS_ENTRY();

	while (mailbox[id].count >= MBOX_SIZE) {
		if (!wait) {
			SYS_EXIT(s);
			return -ERR_NOBLOCK;
		}
		active_thread->status = THREAD_BLOCKED;
		queue_push(&mailbox[id].wait_tx, active_thread);
		SVC();
	}

	in = mailbox[id].index_in;
	mailbox[id].data[in] = data;
	mailbox[id].count += 1;
	++in;
	if (in >= MBOX_SIZE)
		in = 0;
	mailbox[id].index_in = in;

	tmp = queue_pop(&mailbox[id].wait_rx);
	if (tmp) {
		tmp->status = THREAD_READY;
		tmp->param = tmp->prio;
		queue_insert(&ready_queue, tmp);
		if (get_mode() == ARM_IRQ_MODE)
			rtos_sched();
		else
			SVC();
	}
	SYS_EXIT(s);
	return ERR_OK;
}

int mbox_recv(uint id, int *data, uint wait) {
	if (!data || id >= MBOX_COUNT)
		return -ERR_PARAMS;

	if (wait && get_mode() == ARM_IRQ_MODE)
		return -ERR_ISR;

	thread *tmp;
	uint out;
	uint s = SYS_ENTRY();

	while (mailbox[id].count == 0) {
		if (!wait) {
			SYS_EXIT(s);
			return -ERR_NOBLOCK;
		}
		active_thread->status = THREAD_BLOCKED;
		queue_push(&mailbox[id].wait_rx, active_thread);
		SVC();
	}

	out = mailbox[id].index_out;
	*data = mailbox[id].data[out];
	mailbox[id].count -= 1;
	++out;
	if (out >= MBOX_SIZE)
		out = 0;
	mailbox[id].index_out = out;

	tmp = queue_pop(&mailbox[id].wait_tx);
	if (tmp) {
		tmp->status = THREAD_READY;
		tmp->param = tmp->prio;
		queue_insert(&ready_queue, tmp);
		if (get_mode() == ARM_IRQ_MODE)
			rtos_sched();
		else
			SVC();
	}
	SYS_EXIT(s);
	return ERR_OK;
}
