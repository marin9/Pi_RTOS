#include "api.h"

static void Sleep(int ms){
	TaskSleep(TaskSelf(), ms);
}

static void T1(){
	int n=0;
	while(1){
		Printf("T1 %d\r\n", n++);
		Sleep(500);
	}
}

static void T2(){
	int n=0;
	while(1){
		Printf("T2   %d\r\n", n++);
		Sleep(2000);
	}
}

static void TM(){
	time_t t;
	while(1){
		TimeGet(&t);
		Printf("Time: %d s\r\n", t.sec);
		Sleep(1000);
	}
}

static void RD(){
	char buffer[8];
	while(1){
		Gets(buffer, 1);
		Printf("[%s]\r\n", buffer);
	}
}

void test_mix(){
	TaskCreate(TM, 0, 0);
	TaskCreate(T1, 0, 0);
	TaskCreate(T2, 0, 0);
	TaskCreate(RD, 0, 0);
}