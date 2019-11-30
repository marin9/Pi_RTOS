#include "task.h"
#include "device.h"

//	TEST:
//	sem_take
//	sem_give

static void task1(){
	int i;
	while(1){
		sem_take(1);
		for(i=0;i<5;++i){
			uart_print("T 1\r\n");
			task_sleep(1000);
		}
		sem_give(1);
		task_sleep(1000);
	}
}

static void task2(){
	int i;
	while(1){
		sem_take(1);
		for(i=0;i<5;++i){
			uart_print("T  2\r\n");
			task_sleep(1000);
		}
		sem_give(1);
		task_sleep(1000);
	}
}

static void task3(){
	int i;
	while(1){
		sem_take(1);
		for(i=0;i<5;++i){
			uart_print("T   3\r\n");
			task_sleep(1000);
		}
		sem_give(1);
		task_sleep(1000);
	}
}


void test3(){
	sem_init(1, 1);
	task_start(task1, 0);
	task_start(task2, 0);
	task_start(task3, 0);
}
