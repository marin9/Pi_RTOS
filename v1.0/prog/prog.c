#include "api.h"
#include "lib.h"
#include "device.h"

/*
static int N[]={1, 2, 3, 4};


static void func(void *p){
	int i;
	int n=*((int*)p);
	while(1){	
		uart_printhex(Task_self());
		uart_print("\r\n");
		for(i=0;i<50000000;++i){
			asm volatile("");
		}

		if(n==1 || n==2){
			Task_yield();
		}
	}
}

void test_task(){
	int t1, t2, t3, t4;

	t1=Task_create(func, &N[0]);
	t2=Task_create(func, &N[1]);
	t3=Task_create(func, &N[2]);
	t4=Task_create(func, &N[3]);

	if(t1<0 || t2<0 || t3<0 || t4<0){
		uart_print("Thread create fail.\r\n");
	}

	while(1){
		int i;
		uart_print("M  ");
		uart_printhex(Task_count());
		uart_print("\r\n");
		for(i=0;i<100000000;++i){
			asm volatile("");
		}
		if(Task_exit(t2)<0) uart_print("ERR: 1\r\n");
		if(Task_exit(50)<0) uart_print("ERR: 2\r\n");
		Task_yield();
		break;
	}
}
*/


/*
queue_t w;

void func2(){
	int i;

	uart_printhex(Task_self());
	uart_print("\r\n");
	Task_wait(&w);

	while(1){	
		uart_printhex(Task_self());
		uart_print("\r\n");
		for(i=0;i<90000000;++i){
			asm volatile("");
		}
	}
}

void test_wait(){
	int i;
	queue_init(&w);

	Task_create(func2, 0);
	Task_create(func2, 0);

	for(i=0;i<900000000;++i){
		asm volatile("");
	}
	Task_wakeup_all(&w);
	//Task_wakeup(&w);
}*/


static int t1=1000;
static int t2=500;

void func3(void *p){
	int t=*((int*)p);
	while(1){
		if(t==t1) uart_print("  ");
		uart_printhex(Task_self());
		uart_print("\r\n");
		Sleep(t);
	}
}

void test_time(){
	Task_create(func3, &t1);
	Task_create(func3, &t2);
	while(1){
		uart_print("-------------\r\n");
		Sleep(200);
	}
}



void main(){
	//test_task();
	//test_wait();
	test_time();
}
