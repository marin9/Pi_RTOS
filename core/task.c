#include "cpu.h"
#include "task.h"
#include "types.h"
#include "device.h"
#include "interrupt.h"


#define TASK_DORMANT	0
#define TASK_READY		1
#define TASK_SLEEP 		2
#define TASK_BLOCKED	3

typedef struct task_t{
	uint *sp;
	uint id;
	uint prio;
	uint sleep;
	uint status;
	struct task_t *next;
	struct task_t *prev;
} task_t;

typedef struct{
	task_t *first;
	task_t *last;
} task_queue_t;

typedef struct{
	uint value;
	task_queue_t wait_q;
} sem_t;


static uint sched_running;
static uint active_task;
static task_queue_t sleep_queue;
static task_queue_t queue_ready[PRIO_COUNT];
static task_t task[TASK_COUNT];
static sem_t sem[SEM_COUNT];
static char stack[TASK_COUNT][STACK_SIZE] \
	__attribute__((aligned(8)));



static void task_queue_init(task_queue_t *q){
	q->first=0;
	q->last=0;
}

static void task_enqueue(task_queue_t *q, task_t *t){
	if(q->first==0){
		t->next=0;
		t->prev=0;

		q->first=t;
		q->last=t;
	}else{
		t->next=0;
		t->prev=q->last;
		q->last->next=t;
		q->last=t;
	}
}

static task_t* task_dequeue(task_queue_t *q){
	task_t *tmp=q->first;

	if(!tmp){
		return 0;
	}

	q->first=tmp->next;

	if(q->first)
		q->first->prev=0;
	else
		q->last=0;

	if(q->first)

	tmp->next=0;
	tmp->prev=0;
	return tmp;
}

static task_t* task_peek(task_queue_t *q){
	return q->first;
}

static void task_enqueue_sleep(task_queue_t *q, task_t *t, uint ticks){
	task_t *tmp;
	task_t *prev;

	t->sleep=ticks;

	if(!q->first){
		q->first=t;
		q->last=t;
		t->next=0;
		t->prev=0;
		return;
	}

	tmp=q->first;
	prev=0;

	while(tmp && (t->sleep >= tmp->sleep)){
		t->sleep -= tmp->sleep;
		prev=tmp;
		tmp=tmp->next;
	}

	if(!tmp){
		// add at end
		prev->next=t;
		t->prev=prev;
		t->next=0;

		q->last=t;
	}else{
		// insert and dec
		tmp->sleep -= t->sleep;

		if(prev){
			prev->next=t;
			tmp->prev=t;
			t->next=tmp;
			t->prev=prev;
		}else{
			// add first
			q->first=t;

			tmp->prev=t;
			t->next=tmp;
			t->prev=0;
		}
	}
}

static void task_dequeue_sleep(task_queue_t *q, task_t *t){
	if(t==q->first){
		q->first=t->next;
		if(!q->first){
			q->first=0;
			q->last=0;
		}else{
			q->first->prev=0;
			q->first->sleep += t->sleep;
		}
	}else if(t==q->last){
		q->last=t->prev;
		q->last->next=0;
	}else{
		task_t *next=t->next;
		task_t *prev=t->prev;

		prev->next=next;
		next->prev=prev;

		next->sleep += t->sleep;
	}
	t->next=0;
	t->prev=0;
}

static void task_yield(){
	int i;
	task_t *old;
	task_t *new;

	if(!sched_running)
		return;

	old=&task[active_task];
	if(old->status==TASK_READY)
		task_enqueue(&queue_ready[old->prio], old);

	for(i=0;i<PRIO_COUNT;++i){
		new=task_dequeue(&queue_ready[i]);
		if(new){
			break;
		}
	}

	active_task=new->id;
	context_switch(old, new);
}

static void task_block(task_queue_t *q, uint stat){
	task_t *old;

	old=&task[active_task];
	old->status=stat;
	task_enqueue(q, old);
	task_yield();
}

static int task_release(task_queue_t *q){
	task_t *t=task_dequeue(q);
	if(!t){
		return -1;
	}

	task_enqueue(&queue_ready[t->prio], t);
	t->status=TASK_READY;
	return 0;
}

static void task_idle(){
	while(1);
}

static void task_tick(){
	timer_set(TICK_TIME);

	task_t *t=sleep_queue.first;
	if(t){
		if(t->sleep)
			t->sleep -= 1;

		while(t && (t->sleep < 1)){
			task_dequeue_sleep(&sleep_queue, t);
			task_enqueue(&queue_ready[t->prio], t);
			t->status=TASK_READY;
			t=sleep_queue.first;
		}
	}
	task_yield();
}


void task_init(){
	int i;
	for(i=0;i<PRIO_COUNT;++i){
		task_queue_init(&queue_ready[i]);
	}
	for(i=0;i<TASK_COUNT;++i){
		task[i].status=TASK_DORMANT;
	}

	active_task=0;
	sched_running=0;
	task_queue_init(&sleep_queue);
	task_start(task_idle, PRIO_COUNT-1);

	interrupt_register(SYSTMR1_IRQ, task_tick);
	pic_enable(SYSTMR1_IRQ);
	timer_set(TICK_TIME);
}

void task_sched_start(){
	int i;
	task_t *new;

	for(i=0;i<PRIO_COUNT;++i){
		new=task_dequeue(&queue_ready[i]);
		if(new){
			break;
		}
	}

	sched_running=1;
	active_task=new->id;
	context_switch(0, new);
}

// ##############################
// Task functions
// ##############################

void task_start(func fn, uint pr){
	int i;
	interrupts_disable();

	for(i=0;i<TASK_COUNT;++i){
		if(!task[i].status){
			if(i>=TASK_COUNT){
				return;
			}else{
				break;
			}
		}
	}

	task[i].id=i;
	task[i].prio=pr;
	task[i].sp=(uint*)(stack[i]+STACK_SIZE-4);
	task[i].sp-=sizeof(context_t);
	task[i].status=TASK_READY;

	context_t *ctx=(context_t*)task[i].sp;
	context_create(ctx, fn, task_exit);
	task_enqueue(&queue_ready[pr], &task[i]);
	interrupts_enable();
}

void task_sleep(uint ticks){
	task_t *t;
	interrupts_disable();
	t=&task[active_task];
	t->status=TASK_SLEEP;
	task_enqueue_sleep(&sleep_queue, t, ticks);
	task_yield();
	interrupts_enable();
}

void task_exit(){
	interrupts_disable();
	task[active_task].status=TASK_DORMANT;
	task_yield();
}

// ##############################
// Semaphore functions
// ##############################

void sem_init(uint id, uint v){
	if(id>=SEM_COUNT)
		return;

	interrupts_disable();

	sem[id].value=v;
	task_queue_init(&sem[id].wait_q);
	interrupts_enable();
}

void sem_take(uint id){
	if(id>=SEM_COUNT)
		return;

	interrupts_disable();

	if(sem[id].value > 0){
		sem[id].value -= 1;
	}else{
		task_block(&sem[id].wait_q, TASK_BLOCKED);
	}
	interrupts_enable();
}

void sem_give(uint id){
	if(id>=SEM_COUNT)
		return;

	interrupts_disable();

	if(!task_peek(&sem[id].wait_q)){
		sem[id].value += 1;
	}else{
		task_release(&sem[id].wait_q);
		task_yield();
	}
	interrupts_enable();
}

int sem_try(uint id){
	int ret;
	if(id>=SEM_COUNT)
		return -1;

	interrupts_disable();

	if(sem[id].value > 0){
		sem[id].value -= 1;
		ret=0;
	}else{
		ret=1;
	}
	interrupts_enable();
	return ret;
}
