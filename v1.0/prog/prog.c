//TODO #include "api.h"
#include "device.h"

#include "kernel.h"

static int N[]={1, 2, 3, 4};

static void func(void *p){
	int i;
	int n=*((int*)p);
	while(1){	
		uart_printhex(n);
		uart_print("\r\n");
		for(i=0;i<50000000;++i){
			asm volatile("");
		}
		if(n==1 || n==3){
			break;
		}
	}
}


void main(){
	int t1, t2, t3, t4;

	cpu_cli();
	t1=task_create(func, &N[0]);
	t2=task_create(func, &N[1]);
	t3=task_create(func, &N[2]);
	t4=task_create(func, &N[3]);
	cpu_sti();

	if(t1<0 || t2<0 || t3<0 || t4<0){
		uart_print("Thread create fail.\r\n");
	}

	while(1){
		int i;
		uart_print("M\r\n");
		for(i=0;i<50000000;++i){
			asm volatile("");
		}	
		break;
	}
}
