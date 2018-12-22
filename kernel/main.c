#include "stdio.h"
#include "cpu.h"
#include "irq.h"
#include "mem.h"
#include "time.h"
#include "task.h"
#include "prog.h"


void main_thread(){
	//test_sem();
}


void startup(){
	stdio_init();
	cpu_init();
	irq_init();
	mem_init();
	time_init();
	task_init(main_thread);
}

