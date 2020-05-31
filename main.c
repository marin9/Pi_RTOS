#include "rpi.h"
#include "lib.h"
#include "os.h"

#define READY_LED   21

/*
static void delay(uint us) {
    us += timer_get();
    while (timer_get() < us);
}
*/
static void t0() {
    while (1) {
        uart_print("Task 0\r\n");
        task_sleep(1000);
    }
}

void setup() {
    // user code
    pic_init();
    uart_init();
    timer_init();
    //spi_init(32);
    os_init();
    gpio_mode(READY_LED, GPIO_OUT);
    gpio_write(READY_LED, 1);

    //delay(4000000);
    uart_print("OK\r\n");
    task_create(t0, 0, 0);
    os_start();
}
