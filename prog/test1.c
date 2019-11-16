#include "task.h"
#include "device.h"

//	TEST:
//	task_create
//	task_sleep
//  task_yield

static void task1(){
	while(1){
		uart_print("T 1\r\n");
		task_sleep(1000);
	}
}

static void task2(){
	while(1){
		uart_print("T  2\r\n");
		task_sleep(2000);
	}
}

static void task3(){
	while(1){
		uart_print("T   3\r\n");
		task_sleep(3000);
	}
}


void test1(){
	timer_wait(5000000);
	task_start(task1, 0);
	task_start(task2, 0);
	task_start(task3, 0);
}
