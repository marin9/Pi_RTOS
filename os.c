#include "os.h"
#include "rpi.h"

#define TASK_UNUSED		0
#define TASK_READY		1
#define TASK_BLOCKED	2

#define CPU_LED			20

#define SYS_ENTRY()		volatile uint i_flag = cpu_interrupts(1)
#define SYS_EXIT()		cpu_interrupts(i_flag)

static volatile uint sched_run;
static volatile uint sys_time;
static task_t *active_task;
static queue_t ready_queue[PRIO_COUNT];
static queue_t sleep_queue;
static task_t task[TASK_COUNT + 1];
static char stack[TASK_COUNT][STACK_SIZE] \
	__attribute__((aligned(8)));



static void queue_init(queue_t *q) {
	q->first = 0;
	q->last = 0;
}

static void queue_push(queue_t *q, task_t *t) {
	if (q->first == 0) {
		t->next = 0;
		q->first = t;
		q->last = t;
	} else {
		t->next = 0;
		q->last->next = t;
		q->last = t;
	}
}

static void queue_pushsort(queue_t *q, task_t *t) {
	task_t *tmp;

	if (q->first == 0) {
		q->first = t;
		q->last = t;
		t->next = 0;
	} else {
		tmp = q->first;
		if (t->param < tmp->param) {
			t->next = tmp;
			q->first = t;
		} else {
			while (tmp->next && (t->param > tmp->next->param))
				tmp = tmp->next;

			if (tmp->next == 0) {
				tmp->next = t;
				t->next = 0;
				q->last = t;
			} else {
				t->next = tmp->next;
				tmp->next = t;
			}
		}
	}
}

static task_t* queue_pop(queue_t *q) {
	task_t *tmp = q->first;
	if (!tmp)
		return 0;

	q->first = tmp->next;
	if (!q->first)
		q->last = 0;

	return tmp;
}

static task_t* queue_peek(queue_t *q) {
	return q->first;
}


static void task_yield() {
	int i;
	task_t *prev = active_task;

	if (!sched_run)
		return;

	if (prev && prev->status == TASK_READY)
		queue_push(&ready_queue[prev->prio], prev);

	active_task = 0;
	for (i = 0; i < PRIO_COUNT && !active_task; ++i)
		active_task = queue_pop(&ready_queue[i]);

	if (active_task == &task[0])
		gpio_write(CPU_LED, 0);
	else
		gpio_write(CPU_LED, 1);

	cpu_switch(prev, active_task);
}

static void os_tick_handler() {
	int sched;
	task_t *t;

	++sys_time;
	timer_set(TICK_TIME);

	if (!sched_run)
		return;

	sched = 0;
	t = queue_peek(&sleep_queue);
	while (t) {
		if (sys_time >= t->param) {
			t = queue_pop(&sleep_queue);
			t->status = TASK_READY;
			queue_push(&ready_queue[t->prio], t);
			t = queue_peek(&sleep_queue);
			sched = 1;
		} else {
			break;
		}
	}

	if (sched)
		task_yield();
}

static void os_gpio_handler() {
	gpio_irq_handler();
	task_yield();
}

static void idle() {
	while (1) {
		SYS_ENTRY();
		task_yield();
		SYS_EXIT();
	}
}


void os_init() {
	int i;
	sched_run = 0;
	active_task = 0;
	sys_time = 0;

	for (i = 0; i < PRIO_COUNT; ++i)
		queue_init(&ready_queue[i]);

	for (i = 0; i < TASK_COUNT; ++i)
		task[i].status = TASK_UNUSED;

	queue_init(&sleep_queue);
	task_create(idle, 0, PRIO_COUNT - 1);	
}

void os_start() {
	sched_run = 1;
	gpio_mode(CPU_LED, GPIO_OUT);
	pic_register(SYSTMR1_IRQ, os_tick_handler);
	pic_register(GPIO_IRQ_0, os_gpio_handler);
	timer_set(TICK_TIME);
	task_yield();
}

uint os_ticks() {
	return sys_time;
}


int task_create(func fn, void *arg, uint prio) {
	int i;
	SYS_ENTRY();

	for (i = 0; i < TASK_COUNT; ++i)
		if (!task[i].status)
			break;

	if (i >= TASK_COUNT) {
		SYS_EXIT();
		return -ERR_NORES;
	}

	if (prio >= PRIO_COUNT) {
		SYS_EXIT();
		return -ERR_PARAMS;
	}

	task[i].prio = prio;
	task[i].status = TASK_READY;
	task[i].sp = (uint*)(stack[i] + STACK_SIZE - 4);
	task[i].sp -= sizeof(context_t);

	context_t *ctx = (context_t*)task[i].sp;
	ctx->cpsr = CPSR_MODE_SYS;
  	ctx->lr = (uint)task_term;
  	ctx->pc = (uint)fn;
  	ctx->r0 = (uint)arg;

	queue_push(&ready_queue[prio], &task[i]);

	if (sched_run)
		task_yield();

	SYS_EXIT();
	return ERR_OK;
}

void task_sleep(uint ticks) {
	SYS_ENTRY();
	if (ticks) {
		active_task->param = ticks + sys_time;
		active_task->status = TASK_BLOCKED;
		queue_pushsort(&sleep_queue, active_task);
	}
	task_yield();
	SYS_EXIT();
}

void task_term() {
	DISABLE_INTR();
	active_task->status = TASK_UNUSED;
	task_yield();
}


int sem_init(sem_t *s, uint val, uint max) {
	if (!s || !max || (val > max))
		return -ERR_PARAMS;

	s->max = max;
	s->value = val;
	queue_init(&s->waitq);
	return ERR_OK;
}

int sem_wait(sem_t *s, uint wait) {
	int ret = ERR_OK;

	if (!s)
		return -ERR_PARAMS;

	SYS_ENTRY();
	if (s->value > 0) {
		s->value -= 1;
	} else if (!wait) {
		ret = -ERR_NOBLOCK;
	} else {
		active_task->status = TASK_BLOCKED;
		queue_push(&(s->waitq), active_task);
		task_yield();
	}
	SYS_EXIT();
	return ret;
}

int sem_signal(sem_t *s) {
	int ret = ERR_OK;
	task_t *tmp;

	if (!s)
		return -ERR_PARAMS;

	SYS_ENTRY();
	tmp = queue_pop(&(s->waitq));
	if (tmp) {
		tmp->status = TASK_READY;
		queue_push(&ready_queue[tmp->prio], tmp);
		task_yield();
	} else if (s->value < s->max) {
		s->value += 1;
	} else {
		ret = ERR_OVERFLOW;
	}
	SYS_EXIT();
	return ret;
}


int mbox_init(mbox_t *mb, int *buff, uint size) {
	if (!mb || !buff || !size)
		return -ERR_PARAMS;

	mb->buff = buff;
	mb->size = size;
	mb->count = 0;
	mb->x_in = 0;
	mb->x_out = 0;
	queue_init(&(mb->wait_in));
	queue_init(&(mb->wait_out));
	return ERR_OK;
}

int mbox_send(mbox_t *mb, int data, uint wait) {
	task_t *tmp;

	if (!mb)
		return -ERR_PARAMS;

	SYS_ENTRY();
	while (mb->count >= mb->size) {
		if (wait) {
			active_task->status = TASK_BLOCKED;
			queue_push(&(mb->wait_in), active_task);
			task_yield();
		} else {
			return -ERR_NOBLOCK;
		}
	}

	mb->buff[mb->x_in] = data;
	mb->count += 1;
	mb->x_in = (mb->x_in + 1) % mb->size;

	tmp = queue_pop(&(mb->wait_out));
	if (tmp) {
		tmp->status = TASK_READY;
		queue_push(&ready_queue[tmp->prio], tmp);
		task_yield();
	}
	SYS_EXIT();
	return ERR_OK;
}

int mbox_recv(mbox_t *mb, int *data, uint wait) {
	task_t *tmp;

	if (!mb)
		return -ERR_PARAMS;

	SYS_ENTRY();
	while (mb->count == 0) {
		if (wait) {
			active_task->status = TASK_BLOCKED;
			queue_push(&(mb->wait_out), active_task);
			task_yield();
		} else {
			return -ERR_NOBLOCK;
		}
	}

	*data = mb->buff[mb->x_out];
	mb->count -= 1;
	mb->x_out = (mb->x_out + 1) % mb->size;

	tmp = queue_pop(&(mb->wait_in));
	if (tmp) {
		tmp->status = TASK_READY;
		queue_push(&ready_queue[tmp->prio], tmp);
		task_yield();
	}

	SYS_EXIT();
	return ERR_OK;
}
