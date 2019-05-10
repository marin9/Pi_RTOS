#include "types.h"
#include "core.h"
#include "dev.h"


void* Malloc(uint size){
	void *s;
	cpu_cli();
	s=memory_alloc(size);
	cpu_sti();
	return s;
}

void Mfree(void *p){
	cpu_cli();
	memory_free(p);
	cpu_sti();
}

uint Time(){
	return time_get();
}

void Sleep(uint ms){
	cpu_cli();
	time_wait(ms);
	cpu_sti();
}

int Task_create(void *func, void *arg){
	int s;
	cpu_cli();
	s=task_create(func, arg);
	cpu_sti();
	return s;
}

int Task_wait(queue_t *q){
	int s;
	cpu_cli();
	s=task_wait(q);
	cpu_sti();
	return s;
}

int Task_wakeup(queue_t *q){
	int s;
	cpu_cli();
	s=task_wakeup(q);
	cpu_sti();
	return s;
}

int Task_wakeup_all(queue_t *q){
	int s;
	cpu_cli();
	s=task_wakeup_all(q);
	cpu_sti();
	return s;
}

int Task_exit(uint id){
	int s;
	cpu_cli();
	s=task_exit(id);
	cpu_sti();
	return s;
}

int Task_count(){
	return task_count();
}

int Task_self(){
	return task_self();
}

void Task_yield(){
	cpu_cli();
	task_yield();
	cpu_sti();
}

int Sem_init(sem_t *s, int val, int type){
	return sem_init(s, val, type);
}

int	Sem_wait(sem_t *s){
	int v;
	cpu_cli();
	v=sem_wait(s);
	cpu_sti();
	return v;
}

int	Sem_post(sem_t *s){
	int v;
	cpu_cli();
	v=sem_post(s);
	cpu_sti();
	return v;
}

int	Sem_trywait(sem_t *s){
	int v;
	cpu_cli();
	v=sem_trywait(s);
	cpu_sti();
	return v;
}
