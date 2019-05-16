#include "api.h"
#include "lib.h"
#include "dev.h"


static void work(uint w){
	while(--w){
		asm volatile("");
	}
}

//#############################
//###		Test count
//#############################

static int p=1;
static int buffer[5];
static sem_t s1, s2;


static void proizv(void *arg){
	int i=0, n=*((int*)arg);

	while(1){
		++n;
		Sleep(300);

		uart_print("+++:\t");
		uart_printhex(n);
		uart_print("\r\n");

		//Sem_wait(&s1);
		buffer[i++]=n;
		if(i>=5){
			i=0;
		}
		//Sem_post(&s2);
	}
}

static void trosi(){
	int v, iz=0;
	while(1){
		//Sem_wait(&s2);
		v=buffer[iz++];
		if(iz>=5){
			iz=0;
		}
		//Sem_post(&s1);

		uart_print("-:\t");
		uart_printhex(v);
		uart_print("\r\n");
		Sleep(1000);
	}
}


void test_lock2(){
	Sem_init(&s1, 5, SEM_COUNT);
	Sem_init(&s2, 0, SEM_COUNT);

	Task_create(proizv, &p);
	Task_create(trosi, 0);
}



//#############################
//###		Test bin
//#############################

static sem_t s1;


static void taskf(){
	int i;
	while(1){
		//while(Sem_trywait(&s1));

		Sem_wait(&s1);
		for(i=0;i<5;++i){
			work(20000000);
			uart_printhex(Task_self());
			uart_print("\r\n");
		}
		Sem_post(&s1);

		for(i=0;i<5;++i){
			work(20000000);
		}
	}
}

void test_lock1(){
	Sem_init(&s1, 1, SEM_BIN);
	Task_create(taskf, 0);
	Task_create(taskf, 0);
	Task_create(taskf, 0);
	Task_create(taskf, 0);
	Task_create(taskf, 0);
}

