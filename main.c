#include "rpi.h"
#include "dev.h"
#include "lib.h"
#include "os.h"

 /*
static void t0() {
    while (1) {
        uart_print("Task 100\r\n");
        task_sleep(100);
    }
}

static void t1() {
    while (1) {
        uart_print("Task   300\r\n");
        task_sleep(300);
    }
}

static void t2() {
    while (1) {
        uart_print("Task     1000\r\n");
        task_sleep(1000);
    }
}

static void t3() {
    while (1) {
        uart_print("Task         1900\r\n");
        task_sleep(1900);
    }
}

void test1() {
    task_create(t0, 0, 0);
    task_create(t1, 0, 0);
    task_create(t2, 0, 0);
    task_create(t3, 0, 0);
}
 */

/*
static sem_t s;

static void tsk(void *arg) {
    int i;
    char *m = (char*)arg;

    while (1) {
        sem_wait(&s, 1);
        for (i = 0; i < 5; ++i) {
            uart_print(m);
            task_sleep(500);
        }
        sem_signal(&s);
        task_sleep(100);    
    }
}

void test2() {
    sem_init(&s, 1, 1);
    task_create(tsk, "A\r\n", 0);
    task_create(tsk, " B\r\n", 0);
    task_create(tsk, "  C\r\n", 0);
}
*/



void main() {
    pic_init();
    timer_init();
    uart_init(115200);
    i2c_init(400000);
    spi_init(100000);
    pwm_init(40000, 0x100);
    ssd1306_init();
    os_init();

    ssd1306_clear(0);




    lcd1602_init(21, 20, 16,
                26, 19, 13, 6);


    lcd1602_setcursor(1, 5);
    lcd1602_print("OK");
    
    while(1);

    os_start();
}

