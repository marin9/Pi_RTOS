#include "types.h"
#include "kernel.h"
#include "device.h"


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

int Task_create(void *func, void *arg){
	int s;
	cpu_cli();
	s=task_create(func, arg);
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
