#include "api.h"
#include "lib.h"
#include "dev.h"


//TODO add pipe or msgq



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



/*
static int t1=500;
static int t2=100;

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
		uart_print("-------------");
		uart_printhex(Time()/1000);
		uart_print("-------------\r\n");
		Sleep(1000);
	}
}*/




/*
// Test count
static int p=1;
static int buffer[5];
static sem_t s1, s2;


static void proizv(void *arg){
  int n=*((int*)arg);
  int i=0;
  while(1){
    ++n;
    Sleep(300);

    uart_print("+++:\t");
    uart_printhex(n);
    uart_print("\r\n");

    //Sem_wait(&s1);
    buffer[i]=n;
    ++i;
    if(i>=5) i=0;
    //Sem_post(&s2);
  }
}

static void trosi(){
  int v;
  int iz=0;
  while(1){
    //Sem_wait(&s2);
    v=buffer[iz];
    ++iz;
    if(iz>=5) iz=0;
    //Sem_post(&s1);

    uart_print("-:\t");
    uart_printhex(v);
    uart_print("\r\n");

    Sleep(1000);
  }
}


void test_lock(){
  Sem_init(&s1, 5, SEM_COUNT);
  Sem_init(&s2, 0, SEM_COUNT);
  Task_create(proizv, &p);
  Task_create(trosi, 0);
  uart_print("Main exit\r\n");
}*/


// Test bin
/*
static sem_t s1;

static void delay(){
	int i;
	for(i=0;i<10000000;++i){
		asm volatile("");
	}
}

static void tx(){
	int i;
	while(1){
		Sem_wait(&s1);
		//while(Sem_trywait(&s1));
		for(i=0;i<5;++i){
			delay();
			uart_printhex(Task_self());
			uart_print("\r\n");
		}
		Sem_post(&s1);
		for(i=0;i<5;++i){
			delay();
		}
	}
}

static void test_lock(){
	int i;
	Sem_init(&s1, 1, SEM_BIN);
	for(i=0;i<5;++i){
		Task_create(tx, 0);
	}
	uart_print("Main exit\r\n");
}
*/


// test memory
static int n=0;
static sem_t sem;

void fn(){
	//Sem_wait(&sem);
	++n;
	//Sem_post(&sem);
	uart_printhex(n);
	uart_print("\r\n");
}

void test_memory(){
	int i, s;
	Sem_init(&sem, 1, SEM_BIN);
	while(1){
		for(i=0;i<50;++i){
			s=Task_create(fn, 0);
			if(s<0){
				uart_print("ERROR: task_create fail: ");
				uart_printhex(s);
				uart_print("\r\n");
				return;
			}
		}
		Sleep(200);
	}
}

void main(){
	test_memory();
	//test_task();
	//test_wait();
	//test_time();
	//test_lock();
}
