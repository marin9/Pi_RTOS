#include "os.h"
#include "rpi.h"
#include "lib.h"

//  Test functions:
//      sem_init
//      sem_wait
//      sem_signal


#define CONSUMERS	2
#define PRODUCERS	2
#define BUFF_SIZE	3


sem_t s_full;
sem_t s_empty;
sem_t s_pr;
sem_t s_co;

int n;
int in, out, buff[BUFF_SIZE];


void consumer(void *arg) {
    int id = (int)arg;

    while (1) {
        sem_wait(&s_full, 1);
        sem_wait(&s_co, 1);

        uart_print("C:\t\tid: ");
        uart_hex(id);
        uart_print("\t");
        uart_hex(buff[out]);
        uart_print("\r\n");

        out = (out + 1) % BUFF_SIZE;

        sem_signal(&s_co);
        sem_signal(&s_empty);
        task_sleep(1000);
    }
}

void producer(void *arg) {
    int id = (int)arg;

    while (1) {
        sem_wait(&s_empty, 1);
        sem_wait(&s_pr, 1);

        buff[in] = ++n;

        uart_print("Producer:\tid: ");
        uart_hex(id);
        uart_print("\t");
        uart_hex(buff[in]);
        uart_print("\r\n");

        in = (in + 1) % BUFF_SIZE;

        sem_signal(&s_pr);
        sem_signal(&s_full);
        task_sleep(300);
    }
}


void setup() {
    int i;

    uart_init();
	pic_init();
	timer_init();
	os_init();

    n = 0;
    in = 0;
    out = 0;

	sem_init(&s_empty, BUFF_SIZE, BUFF_SIZE);
    sem_init(&s_full, 0, BUFF_SIZE);
    sem_init(&s_pr, 1, 1);
    sem_init(&s_co, 1, 1);

    for (i = 0; i < CONSUMERS; ++i)
        task_create(consumer, (void*)i, 0);

    for (i = 0; i < PRODUCERS; ++i)
        task_create(producer, (void*)i, 0);

    for(i=0;i<30000000;++i)
        asm volatile("nop");
    os_start();
}
