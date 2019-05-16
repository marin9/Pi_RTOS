#include "api.h"
#include "lib.h"
#include "dev.h"


static int N[]={1, 2, 3, 4};


static void work(uint w){
	while(--w){
		asm volatile("");
	}
}


static void func(void *p){
	while(1){
		uart_printhex(*((uint*)p));
		uart_print("\r\n");
		work(50000000);
	}
}

void test_task(){
	int t1, t2, t3, t4;

	t1=Task_create(func, &N[0]);
	t2=Task_create(func, &N[1]);
	t3=Task_create(func, &N[2]);
	t4=Task_create(func, &N[3]);

	if(t1<0 || t2<0 || t3<0 || t4<0){
		uart_print("ERROR: thread create fail.\r\n");
	}
	work(500000000);
}
