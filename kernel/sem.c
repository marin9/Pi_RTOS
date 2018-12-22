#include "sem.h"
#include "mem.h"
#include "task.h"
#include "list.h"


typedef struct{
  int sid;
  int type;
  int value;
  list_t task_list;
  node_t node;
} sem_t;


static int next_sid;
static list_t sem_list;
extern list_t ready_list;
extern task_t *active_task;



static int comp_sid(void *o1, void *o2){
  int sid=*((int*)o1);
  sem_t *t=(sem_t*)o2;
  return t->sid==sid;
}



void sem_init(){
  next_sid=0;
  list_init(&sem_list);
}

int	sem_create(int val, int type){
  if(val<0 || type<0 || type>1){
  	return -2;
  }

  sem_t *sem=(sem_t*)mem_alloc(sizeof(sem_t));
  if(!sem){
  	return -1;
  }

  sem->sid=next_sid++;
  sem->value=val;
  sem->type=type;

  list_init(&(sem->task_list));
  list_addFirst(&sem_list, &(sem->node), sem);
  return sem->sid;
}

int	sem_delete(int sid){
  node_t *node=list_remove(&sem_list, &sid, comp_sid);
  if(!node){
  	return -1;
  }

  sem_t *sem=node->object;
  mem_free(sem);
  return 0;
}

int	sem_wait(int sid){
  node_t *node=list_get(&sem_list, &sid, comp_sid);
  if(!node){
  	return -1;
  }
  sem_t *sem=node->object;

  if(sem->type==SEM_COUNT && sem->value>0){
    sem->value=sem->value-1;
  }else if(sem->type==SEM_BIN && sem->value==1){
  	sem->value=0;
  }else{
    // Put in sem list
    list_addLast(&(sem->task_list), &(active_task->node), active_task);
    active_task->queue=&(sem->task_list);
    active_task->status=TASK_BLOCKED;

	task_run_next();
  }
  return 0;
}

int	sem_trywait(int sid){
  node_t *node=list_get(&sem_list, &sid, comp_sid);
  if(!node){
  	return -1;
  }
  sem_t *sem=node->object;

  if(sem->type==SEM_COUNT && sem->value>0){
    sem->value=sem->value-1;
    return 0;
  }else if(sem->type==SEM_BIN && sem->value==1){
	sem->value=0;
  	return 0;
  }else{
    return 1;
  }
}

int	sem_post(int sid){
  node_t *node=list_get(&sem_list, &sid, comp_sid);
  if(!node){
  	return -1;
  }
  sem_t *sem=node->object;


  if(list_count(&(sem->task_list))==0){
  	if(sem->type==SEM_COUNT){
  		sem->value=sem->value+1;
  	}else{
  		sem->value=1;
  	}  
  }else{
  	// Remove from sem list
    node=list_removeFirst(&(sem->task_list));
    task_t *task=node->object;
    // Insert to run list
    list_addSort(&ready_list, &(task->node), task, comp_prio);
    task->queue=&ready_list;
    task->status=TASK_RUNNING;
  }
  return 0;
}
