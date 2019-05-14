#include "api.h"
#include "lib.h"
#include "dev.h"


static void work(uint w){
	while(--w){
		asm volatile("");
	}
}


static void func(){
	work(5000000);
	uart_print("fin ");
	uart_printhex(Task_self());
	uart_print("\r\n");
}


void test_memory(){
	while(1){
		if(Task_create(func, 0)<0){
			uart_print("ERROR: task_create fail.\r\n");
			return;
		}

		while(Task_count()>50){
			Sleep(200);
		}
	}
}
