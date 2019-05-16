#include "dev.h"
#include "api.h"

static int t1;


static void task_first(){
	int i=500000000;
	Task_yield();
	uart_print("FIRST start\r\n");
	while(i--) asm volatile("");
	uart_print("FIRST end\r\n");
}

static void task_second(){
	uart_print("SECOND wait\r\n");
	Task_join(t1);
	uart_print("SECOND resume\r\n");
	uart_printhex(Task_self());
	uart_print("\r\n");
}


void test_join(){
	t1=Task_create(task_first, 0);	
	Task_create(task_second, 0);
	Task_create(task_second, 0);
	Task_create(task_second, 0);
}

