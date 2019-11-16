#include "cpu.h"
#include "rpi.h"
#include "task.h"
#include "types.h"
#include "device.h"
#include "interrupt.h"


void test1();
void test2();
void test3();


void main(){
	uart_init();
	timer_init();
	pic_init();
	interrupt_init();
	task_init();

	task_start(test3, 0);
	task_sched_start();
}
