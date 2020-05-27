#include "rpi.h"
#include "lib.h"
#include "os.h"


void task0() {
    while (1) {
        uart_print("T 0\r\n");
        task_sleep(200);
    }
}

void task1() {
    while (1) {
        uart_print("T  1\r\n");
        task_sleep(400);
    }
}

void setup() {
    pic_init();
    uart_init();
    timer_init();

    os_init();

    task_create(task0, 0, 0);
    task_create(task1, 0, 0);

    os_start();
}