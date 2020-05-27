#include "os.h"
#include "rpi.h"
#include "lib.h"

//  Test functions:
//      task_create


void ts_a(void *arg) {
    int w;
    while (1) {
    	uart_print((char*)arg);
    	for (w = 0; w < 6000000; ++w)
    		asm volatile("nop");

        uart_print((char*)arg);
        for (w = 0; w < 6000000; ++w)
        	asm volatile("nop");

        uart_print((char*)arg);
        for (w = 0; w < 6000000; ++w)
            asm volatile("nop");

        task_sleep(2000);
    }
}

void ts_b(void *arg) {
    while (1) {
        uart_print((char*)arg);
    }
}

void setup() {
	uart_init();
	pic_init();
	timer_init();
	os_init();

	task_create(ts_a, "Task\tA\r\n",   0);
    task_create(ts_b, "Task\t\tB\r\n", 1);

	os_start();
}
