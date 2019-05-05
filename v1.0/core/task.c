#include "device.h"
#include "kernel.h"
#include "param.h"
#include "types.h"
#include "lib.h"


static int next_id;
static int sched_en=0;
static queue_t all_tasks;
//static queue_t sleep_tasks;
static queue_t ready_tasks;
static task_t *active_task;

//TODO add pipe or msgq


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
	next_id=0;
	active_task=0;
	queue_init(&all_tasks);
	queue_init(&ready_tasks);
//TODOlist_init(&sleep_list);

	// Create idle task and main task
	if(task_create(idle, 0)<0) return -1;
	if(task_create(main, 0)<0) return -2;
	return 0;
}

void start_sched(){
	sched_en=1;
	task_yield();
}


int task_create(void *func, void *arg){
	task_t *task;
	context_t *context;
	if(!func) return -1;

	// Allocate memory for task
	task=memory_alloc(sizeof(task_t));
	if(!task) return -2;
	task->stack=memory_alloc(TASK_STACK);
	if(!(task->stack)){
		memory_free(task);
		return -2;
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
	if(!sched_en) return;


	//time_t current_time;
	//time_get(&current_time);

	// Wake up all task from sleep with elapsed time
	/*while(1){
		// Get first task from list
 		node_t *node=list_getFirst(&sleep_list);
    	if(!node) break;
    	task_t *task=node->object;

    	// If first is not finised with sleep, there is not one
    	if(time_cmp(&current_time, &(task->sleep))<0){
      		break;
      	// Pop first from list and put into ready list
    	}else{
      		list_removeFirst(&sleep_list);
      		list_addSort(&ready_list, &(task->node), task, comp_prio);
      		task->queue=&ready_list;
      		task->status=TASK_RUNNING;
    	}
	}*/
  	task_yield();
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




/*
int task_sleep(int tid, int ms){
	// Set time value descriptor
	time_t delay;
  	delay.sec=ms/1000;
  	delay.usec=ms%1000;
  	delay.usec=delay.usec*1000;

  	time_t current;
  	time_get(&current);

  	// Current task sleep
	if(tid==active_task->tid){
		// Store current task to sleep list
    	time_add(&current, &delay, &(active_task->sleep));
    	list_addSort(&sleep_list, &(active_task->node), active_task, comp_time);
    	active_task->queue=&sleep_list;
    	active_task->status=TASK_SLEEPING;

  		task_run_next();
    	return 0;
    // Other task to sleep
  	}else{
  		// Find selected task
    	node_t *node=list_remove(&ready_list, &tid, comp_tid);
    	if(!node) node=list_remove(&sleep_list, &tid, comp_tid);
    	if(node){
      		task_t *task=node->object;
      		// Set time for sleep
      		time_add(&current, &delay, &(task->sleep));
      		// Put task into sleep list
      		list_addSort(&sleep_list, &(task->node), task, comp_time);
      		task->queue=&sleep_list;
      		task->status=TASK_SLEEPING;
      		return 0;
    	}else{
      		return -1;
    	}
  	}
}

int task_wakeup(int tid){
	// Find selected task
	node_t *node=list_remove(&sleep_list, &tid, comp_tid);
	if(!node) return -1;
	task_t *task=node->object;

	// Insert to run list
	list_addSort(&ready_list, &(task->node), task, comp_prio);
	task->queue=&ready_list;
	task->status=TASK_RUNNING;
	return 0;
}
*/

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
			return -1;
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
