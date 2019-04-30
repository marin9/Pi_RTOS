#include "api.h"

// Test count
/*
int p=10;
int s1, s2;
int buffer[5];

void proizv(void *arg){
  int n=*((int*)arg);
  int i=0;
  while(1){
    n++;
    TaskSleep(TaskSelf(), 300);
    Printf("Proizveo: %d\r\n", n);
    SemWait(s1);
    buffer[i]=n;
    ++i;
    if(i>=5) i=0;
    SemPost(s2);
  }
}

void trosi(){
  int v;
  int iz=0;
  while(1){
    SemWait(s2);
    v=buffer[iz];
    ++iz;
    if(iz>=5) iz=0;
    SemPost(s1);
    Printf("Potrosio:\t%d\r\n", v);
    TaskSleep(TaskSelf(), 1000);
  }
}


void test_sem(){
  s1=SemCreate(5, SEM_COUNT);
  s2=SemCreate(0, SEM_COUNT);
  TaskCreate(proizv, &p, 1);
  TaskCreate(trosi, 0, 1);
  Printf("Main exit\r\n");
}
*/


// Test bin

int s1;

void delay(){
	int i;
	for(i=0;i<50000000;++i){
		asm volatile("");
	}
}

void tx(){
	while(1){
		int i;
		SemWait(s1);
		//while(SemTryWait(s1));
		for(i=0;i<5;++i){
			delay();
			Printf("%d\r\n", TaskSelf());
		}
		SemPost(s1);
		for(i=0;i<5;++i){
			delay();
		}
	}
}


void test_sem(){
	Printf("TEST BIN SEM\r\n");
	s1=SemCreate(1, SEM_BIN);
	int i;
	for(i=0;i<5;++i){
		TaskCreate(tx, 0, 2);
	}
	Printf("Main exit\r\n");
}

