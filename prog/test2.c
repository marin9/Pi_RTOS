#include "task.h"
#include "device.h"

//	TEST:
//	task_prio
//	task_exit

static void task1(){
	int i;
	for(i=0;i<5;++i){
		uart_print("T 1\r\n");
		task_sleep(1000);
	}
	task_exit();
}

static void task2(){
	int i;
	for(i=0;i<2;++i){
		uart_print("T  2\r\n");
		task_sleep(1000);
	}
	task_exit();
}

static void task3(){
	int i;
	for(i=0;i<3;++i){
		uart_print("T   3\r\n");
		task_sleep(1000);
	}
	task_exit();
}

static void task4(){
	while(1){
		uart_print("T\r\n");
		task_sleep(1000);
	}
}

void test2(){
	task_start(task1, 0);
	task_start(task2, 0);
	task_start(task3, 1);
	task_start(task4, 1);
}
