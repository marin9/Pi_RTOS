#include "os.h"
#include "rpi.h"
#include "lib.h"

//  Test functions:
//      task_term
//      task_sleep


void ts0();
void ts1();
void ts2();
void ts3();


void ts0() {
    int i;
    for (i = 0; i < 5; ++i){
        uart_print("Task 0\r\n");
        task_sleep(1000);
    }
    task_create(ts2, 0, 0);
    task_term();
}

void ts1() {
    int i;
    for (i = 0; i < 5; ++i){
        uart_print("Task  1\r\n");
        task_sleep(1000);
    }
    task_create(ts3, 0, 0);
    task_term();
}

void ts2() {
    int i;
    for (i = 0; i < 5; ++i){
        uart_print("Task   2\r\n");
        task_sleep(1000);
    }
    task_create(ts0, 0, 0);
    task_term();
}

void ts3() {
    int i;
    for (i = 0; i < 5; ++i){
        uart_print("Task    3\r\n");
        task_sleep(1000);
    }
    task_create(ts1, 0, 0);
    task_term();
}

void setup() {
	uart_init();
	pic_init();
	timer_init();
	os_init();

	task_create(ts0, 0, 0);
    task_create(ts1, 0, 0);

	os_start();
}
