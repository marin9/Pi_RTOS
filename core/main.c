#include "rpi.h"
#include "cpu.h"
#include "irq.h"
#include "task.h"
#include "types.h"
#include "device.h"



void test1();
void test2();
void test3();


static void run_test(){
	char c;
	uart_print(" 1|2|3 >");
	uart_getc(&c);
	uart_print("\r\n");

	switch(c){
	case '1':
		test1();
		break;
	case '2':
		test2();
		break;
	case '3':
		test3();
		break;
	}
}


void main(){
	uart_init();
	timer_init();
	pic_init();
	irq_init();
	task_init();

	timer_wait(6000000);
	task_start(run_test, 0);
	task_sched_start();
}
