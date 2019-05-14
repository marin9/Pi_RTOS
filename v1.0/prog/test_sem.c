#include "api.h"
#include "lib.h"
#include "dev.h"


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
}