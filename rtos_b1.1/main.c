#include "rpi.h"
#include "rtos.h"



// TEST 1
/*
static char c;

void nonperiodic_thread() {
	char buff[2];

	while (c != 'e') {
		sem_wait(0);
		buff[0] = c;
		buff[1] = 0;
		uart_print(buff);
	}
	thread_term(2);
	thread_term(3);
	thread_term(thread_getid());
}

void periodic_thread(void *arg) {
	while (1) {
		uart_print((char*)arg);
		thread_sleep(300);
	}
}

void irq_user() {
	if (pic_pending(IRQ_UART)) {
		c = uart_getchar();
		sem_signal(0);
	}
}

void test() {
	pic_enable(IRQ_UART);
	uart_intren();
	sem_init(0, 0, 1);
	c = 0;
	thread_create(periodic_thread, "T 1\n", 0);
	thread_create(periodic_thread, "T  2\n", 0);
	thread_create(periodic_thread, "T   3\n", 0);
	thread_create(nonperiodic_thread, 0, 0);
}
*/

/*
// TEST2

#define SEM0 	0

static int use_sem;

void periodic_thread(void *arg) {
	int i;
	while (1) {
		sem_wait(SEM0);
		for (i = 0; i < 7; ++i) {
			uart_print((char*)arg);
			thread_sleep(100);
		}
		sem_signal(SEM0);
		thread_sleep(1000);
	}
}

void irq_user() {
	uint ch;
	if (pic_pending(IRQ_UART)) {
		ch = uart_getchar();
		if (ch == '0')
			use_sem = 0;
		else
			use_sem = 1;
	}
}

void test() {
	sem_init(SEM0, 1, 1);
	pic_enable(IRQ_UART);
	uart_intren();
	use_sem = 1;
	thread_create(periodic_thread, "T 1\n", 0);
	thread_create(periodic_thread, "T  2\n", 0);
	thread_create(periodic_thread, "T   3\n", 0);
	thread_create(periodic_thread, "T    4\n", 0);
	thread_create(periodic_thread, "T     5\n", 0);
}
*/


/*
// TEST 3

void irq_user() {
	uint ch;
	if (pic_pending(IRQ_UART)) {
		ch = uart_getchar();

		if (mbox_send(0, ch, 0))
			uart_print("mbox full.\n");
	}
}

void rx_thread(void *arg) {
	int data;
	while (1) {
		if (mbox_recv(0, &data, 1)) {
			uart_print((char*)arg);
			uart_print(" No data.\n");
			thread_sleep(2000);
			continue;
		}
		uart_print((char*)arg);
		uart_print(" ");
		uart_hex(data);
		uart_print("\n");
		thread_sleep(2000);
	}
}

void test() {
	mbox_init(0);
	pic_enable(IRQ_UART);
	uart_intren();
	thread_create(rx_thread, "A:", 0);
	thread_create(rx_thread, "B:", 0);
}
*/


/*
// TEST4

void irq_user() {
}

void tx_thread() {
	int data = 0;

	while (1) {
		mbox_send(0, data, 1);
		++data;
		thread_sleep(800);
	}
}

void rx_thread(void *arg) {
	int data;
	while (1) {
		mbox_recv(0, &data, 1);
		uart_print((char*)arg);
		uart_print(" ");
		uart_hex(data);
		uart_print("\n");
		thread_sleep(200);
	}
}

void test() {
	mbox_init(0);

	thread_create(rx_thread, "rx(a):", 0);
	thread_create(rx_thread, "rx(b):", 0);
	thread_create(rx_thread, "rx(c):", 0);

	thread_create(tx_thread, "TX(a):", 0);
	thread_create(tx_thread, "TX(b):", 0);
	thread_create(tx_thread, "TX(c):", 0);
}
*/

// TEST 5
/*
void irq_user() {
	if (pic_pending(IRQ_UART)) {
		uart_getchar();
		sem_signal(0);
	}
}

void work() {
	uint t = 2000000;
	while (t--)
		asm volatile ("nop");
}

void low(void *arg) {
	while (1) {
		uart_print((char*)arg);
		work();
	}
}

void high(void *arg) {
	int i;
	while (1) {
		for (i = 0; i < 10; ++i) {
			uart_print((char*)arg);
			work();
		}
		sem_wait(0);
	}
}

void test() {
	pic_enable(IRQ_UART);
	uart_intren();
	sem_init(0, 0, 1);
	thread_create(low, 		"low\n", 3);
	thread_create(high, 	"   high\n", 1);
}
*/


void irq_user() {
}


void setup() {
	uart_init();
	pic_init();
    timer_init();
    rtos_init();

	//thread_create(test, 0, 0);

    rtos_start();
}
