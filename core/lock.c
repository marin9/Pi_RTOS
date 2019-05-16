#include "types.h"
#include "core.h"
#include "lib.h"


int sem_init(sem_t *s, int val, int type){
	if(!s || val<0 || type <0){
		return -ERR_ARGS;
	}
	s->type=type;
	s->value=val;
	queue_init(&(s->wait_tasks));
	return 0;
}

int	sem_wait(sem_t *s){
	if(!s){
		return -ERR_ARGS;
	}

 	if(s->type==SEM_COUNT && s->value>0){
		s->value=s->value-1;
	}else if(s->type==SEM_BIN && s->value==1){
		s->value=0;
	}else{
		task_wait(&(s->wait_tasks));
	}
	return 0;
}

int	sem_post(sem_t *s){
	if(!s){
		return -ERR_ARGS;
	}
	
	if(queue_count(&(s->wait_tasks))==0){
 		if(s->type==SEM_COUNT){
			s->value=s->value+1;
		}else{
			s->value=1;
		}
		return 0;
	}else{
		return task_wakeup(&(s->wait_tasks));
	}
}

int	sem_trywait(sem_t *s){
	if(!s){
		return -ERR_ARGS;
	}
	
	if(s->type==SEM_COUNT && s->value>0){
		s->value=s->value-1;
		return 0;
	}else if(s->type==SEM_BIN && s->value==1){
		s->value=0;
		return 0;
	}else{
		return 1;
	}
}
