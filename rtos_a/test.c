#include "rpi.h"
#include "dev.h"
#include "lib.h"
#include "os.h"

/*
static void t0() {
    while (1) {
        printf("Task 100\r\n");
        task_sleep(100);
    }
}

static void t1() {
    while (1) {
        printf("Task   300\r\n");
        task_sleep(300);
    }
}

static void t2() {
    while (1) {
        printf("Task     1000\r\n");
        task_sleep(1000);
    }
}

static void t3() {
    while (1) {
        printf("Task         1900\r\n");
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

