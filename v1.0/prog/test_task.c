#include "api.h"


void tf(){
	time_t tm;
	while(1){
		int t=TaskSelf();
		TaskSleep(t, 1000*t);

		TimeGet(&tm);
		Printf("TASK ");
		while(t--) Printf(" ");
		Printf("tid=%d\t sec=%d\r\n", TaskSelf(), tm.sec);
	}
}

void pt(){
	while(1){
		int x;
		for(x=0;x<50000000;++x) asm volatile("");
		Printf("%d\n", TaskSelf());
	}
}


void test_task(){
	Printf("TEST TASKS\r\n");

	TaskCreate(tf, 0, 0);
	TaskCreate(tf, 0, 0);
	TaskCreate(tf, 0, 0);
	TaskCreate(tf, 0, 0);
	/*
	TaskSleep(2, 1000*20);

	Printf("Main task sleep: 5 sec\r\n");
	TaskSleep(TaskSelf(), 5000);

	Printf("Task 2 sleep: 10 sec\r\n");
	TaskSleep(2, 10000);

	Printf("Kill task 3\r\n");
	TaskExit(3);

	Printf("Task 2 status: %d\r\n", TaskStatus(2));
	Printf("Wake up task 2\r\n");
	TaskWakeup(2);

	Printf("Task count: %d\r\n", TaskCount());
	
	Printf("Task 2 status: %d\r\n", TaskStatus(2));
	*/
	TaskSleep(TaskSelf(), 1000);
	Printf("Finish main task\r\n");
}