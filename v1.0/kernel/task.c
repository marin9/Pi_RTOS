#include "types.h"
#include "lib.h"


static int next_id;
static int sched_en=0;
static queue_t all_tasks;
static queue_t sleep_tasks;
static queue_t ready_tasks;
static task_t *active_task;

/*


static void task_finish(){
	disable_interrupts();
	// Free the resources used by the old task.
	list_remove(&all_list, &(active_task->tid), comp_tid);
	mem_free(active_task->stack);
	mem_free(active_task);
	// Resume next task
	active_task=0;
	task_scheduler();
}

static void idle(){
	while(1) cpu_suspend();
}





void task_scheduler(){
	if(!sched_run) return;
	// Put the current task back in the run list
	if(active_task){
		list_addSort(&ready_list, &(active_task->node), active_task, comp_prio);
	}

	time_t current_time;
	time_get(&current_time);

	// Wake up all task from sleep with elapsed time
	while(1){
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
	}
  	task_run_next();
}

void task_run_next(){
	// Get the next task to run.
  	node_t *node=list_removeFirst(&ready_list);
  	task_t *new=node->object;
  	task_t *old=active_task; 	
  	// Resume new task
  	active_task=new;
  	context_switch(old, new);
}

void task_init(void (*main)()){
	int s;
	next_tid=0;
	list_init(&all_list);
	list_init(&ready_list);
	list_init(&sleep_list);
	sem_init();

	// Create idle task and main task
	s=task_create(idle, 0, -1);
	ASSERT(s>=0, "Create idle task fail.");
	s=task_create(main, 0, 0);
	ASSERT(s>=0, "Create main task fail.");

	active_task=0;
	sched_run=1;
	task_scheduler();
}




int task_create(void *func, void *arg, int prio){
	ASSERT(func, "Null pointer.");
	// Allocate memory for task
	context_t *cntx;
	task_t *desc=mem_alloc(sizeof(task_t));
	if(!desc) return -1;
	desc->stack=mem_alloc(TASK_STACK_SIZE);
	if(!(desc->stack)){
		mem_free(desc);
	return -1;
	}

	// Initialize task descriptor
	desc->prio=prio;
	desc->context=cntx=desc->stack+TASK_STACK_SIZE-sizeof(context_t);
	desc->tid=next_tid++;

	// Initialize task context
  	memset(cntx, 0, sizeof(context_t));
  	cntx->r0=(unsigned int)arg;
  	cntx->lr=(unsigned int)func;
  	cntx->sp=(unsigned int)task_finish;
  	cntx->cpsr=0x1f; // System mode, irq enabled

  	// Insert task to ready list
  	desc->queue=&ready_list;
  	desc->status=TASK_RUNNING;
  	list_addSort(&ready_list, &(desc->node), desc, comp_prio);
  	list_addLast(&all_list, &(desc->nodeall), desc);
  	return desc->tid;
}

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

int task_status(int tid){
	node_t *node=list_get(&all_list, &tid, comp_tid);
	task_t *task;
	if(!node){
		return TASK_TERMINATED;
	}else{
		task=node->object;
		return task->status;
	} 	
}

int task_exit(int tid){
	if(tid==active_task->tid){
		task_finish();
		return 0;
	}else{
		// Find task
		task_t *task;
		node_t *node=list_get(&all_list, &tid, comp_tid);
		if(!node) return -1;
		else task=node->object;

    	// Remove selected task
    	list_remove(task->queue, &tid, comp_tid);
    	list_remove(&all_list, &tid, comp_tid);
		mem_free(task->stack);
		mem_free(task);
		return 0;
  }
}

int task_count(){
	return list_count(&all_list);
}

int task_self(){
	return active_task->tid;
}

*/