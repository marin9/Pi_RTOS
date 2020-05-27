#include "os.h"
#include "rpi.h"
#include "lib.h"

//  Test functions:
//      interrupts


#define BTN_1	21
#define BTN_2	20
#define BTN_3	16
#define BTN_4	26

static volatile uint last[4];
static sem_t s;


static void work(char *msg) {
	int i, w;
	for (i = 0;i < 7; ++i) {
		sem_wait(&s, 1);
		uart_print(msg);
		for (w = 0; w < 6000000; ++w)
			asm volatile("nop");
		sem_signal(&s);
	}
}


void btn1_handler() {
	if (os_ticks() > last[0] + 200) {
		last[0] = os_ticks();
		task_create(work, " Doing\t\t1\r\n", 3);
		uart_print("Created 1\r\n");
	}
}

void btn2_handler() {
	if (os_ticks() > last[1] + 200) {
		last[1] = os_ticks();
		task_create(work, " Doing\t\t\t2\r\n", 2);
		uart_print("Created 2\r\n");
	}
}

void btn3_handler() {
	if (os_ticks() > last[2] + 200) {
		last[2] = os_ticks();
		task_create(work, " Doing\t\t\t\t3\r\n", 2);
		uart_print("Created 3\r\n");
	}
}

void btn4_handler() {
	if (os_ticks() > last[3] + 200) {
		last[3] = os_ticks();
		task_create(work, " Doing\t\t\t\t\t4\r\n", 1);
		uart_print("Created 4\r\n");
	}
}


void setup() {
	uart_init();
	pic_init();
	timer_init();
	os_init();

	sem_init(&s, 1, 1);

	gpio_mode(BTN_1, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_2, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_3, GPIO_IN | GPIO_PULLUP);
	gpio_mode(BTN_4, GPIO_IN | GPIO_PULLUP);

	last[0] = last[1] = last[2] = last[3] = 0;
	gpio_enint(BTN_1, btn1_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_2, btn2_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_3, btn3_handler, GPIO_FALLING_EDGE);
	gpio_enint(BTN_4, btn4_handler, GPIO_FALLING_EDGE);

	os_start();
}
