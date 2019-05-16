#include "param.h"
#include "types.h"
#include "core.h"
#include "dev.h"
#include "lib.h"


static int next_id;
static int sched_en=0;
static queue_t all_tasks;
static queue_t ready_tasks;
static task_t *active_task;


static void idle(){
	while(1){
		cpu_cli();
		task_yield();
		cpu_sti();
	}
}

static void task_reap(){
	cpu_cli();
	// Free the resources used by the old task.
	queue_remove(&all_tasks, &(active_task->ndall));
	memory_free(active_task->stack);
	memory_free(active_task);
	// Resume next task
	active_task=0;
	task_yield();
}



int task_init(void (*main)()){
	int ret;
	next_id=0;
	active_task=0;
	queue_init(&all_tasks);
	queue_init(&ready_tasks);

	if(!main){
		return -ERR_ARGS;
	}

	// Create idle task and main task
	ret=task_create(idle, 0);
	if(ret<0) return ret;
	ret=task_create(main, 0);
	if(ret<0) return ret;

	return 0;
}

void start_sched(){
	sched_en=1;
	task_yield();
}


int task_create(void *func, void *arg){
	task_t *task;
	context_t *context;
	if(!func){
		return -ERR_ARGS;
	}

	// Allocate memory for task
	task=memory_alloc(sizeof(task_t));
	if(!task){
		return -ERR_NOMEM;
	}
	task->stack=memory_alloc(TASK_STACK);
	if(!(task->stack)){
		memory_free(task);
		return -ERR_NOMEM;
	}

	// Initialize task descriptor
	task->context=context=task->stack+TASK_STACK-sizeof(context_t);
	task->id=next_id++;

	// Initialize task context
  	memset(context, 0, sizeof(context_t));
  	context->r0=(uint)arg;
  	context->lr=(uint)func;
  	context->sp=(uint)task_reap;
  	context->cpsr=0x1f; //System mode, irq enabled

  	// Insert task to ready list
  	task->status=TASK_RUNNING;
  	queue_push(&ready_tasks, &(task->nd), task);
  	queue_push(&all_tasks, &(task->ndall), task);
  	return task->id;
}


void task_sched(){
	if(sched_en){
		task_yield();
	}
}


void task_yield(){
	task_t *old=active_task;
	if(old){
		queue_push(&ready_tasks, &(old->nd), old);
	}
	task_t *new=queue_pop(&ready_tasks);
	active_task=new;
  	context_switch(old, new);
}

int task_exit(uint id){
	if(id==active_task->id){
		task_reap();
		return 0;
	}else{
		// Find task
		task_t *task=queue_peek(&all_tasks);
		while(task && ((task->id)!=id)){
			task=queue_next(&(task->nd));
		}
		if(!task){
			return -ERR_NOOBJ;
		}
    	// Remove selected task
    	queue_remove(&ready_tasks, &(task->nd));
    	queue_remove(&all_tasks, &(task->ndall));
		memory_free(task->stack);
		memory_free(task);
		return 0;
  }
}

int task_count(){
	return queue_count(&all_tasks);
}

int task_self(){
	return active_task->id;
}

int task_wait(queue_t *q){
	if(!q){
		return -ERR_ARGS;
	}
	task_t *current=active_task;
	current->status=TASK_WAITING;
	queue_push(q, &(current->nd), current);
	
	task_t *new=queue_pop(&ready_tasks);
	active_task=new;
  	context_switch(current, new);
  	return 0;
}

int task_wakeup(queue_t *q){
	if(!q){
		return -ERR_ARGS;
	}
	task_t *task=queue_pop(q);
	if(!task){
		return ERR_NOOBJ;
	}
	task->status=TASK_RUNNING;
	queue_push(&ready_tasks, &(task->nd), task);
	task_yield();
	return 0;
}

int task_wakeup_all(queue_t *q){
	if(!q){
		return -ERR_ARGS;
	}
	task_t *task=queue_pop(q);
	if(!task){
		return ERR_NOOBJ;
	}
	while(task){
		task->status=TASK_RUNNING;
		queue_push(&ready_tasks, &(task->nd), task);
		task=queue_pop(q);
	}
	task_yield();
	return 0;
}

int task_exist(uint id){
	// Find task
	task_t *task=queue_peek(&all_tasks);
	while(task && ((task->id)!=id)){
		task=queue_next(&(task->nd));
	}
	if(!task){
		return 0;
	}else{
		return 1;
	}
}

int task_join(uint id){
	if(!task_exist(id)){
		return -ERR_NOOBJ;
	}
	while(task_exist(id)){
		task_yield();
	}
	return 0;
}

