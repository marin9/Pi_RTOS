#include "os.h"
#include "rpi.h"
#include "lib.h"

//  Test functions:
//      mbox_init
//      mbox_send
//		mbox_recv


#define SEND_DELAY	1000
#define RECV_DELAY	100

int buff[5];
mbox_t mb;


static void t0() {
	int x = 0;
	while (1) {
        uart_print("+ ");
        uart_hex(x);
        uart_print("\r\n");
		mbox_send(&mb, x, 1);
		task_sleep(SEND_DELAY);
        ++x;
	}
}

static void t1() {
	int n;
	while (1) {
		mbox_recv(&mb, &n, 1);
		uart_print("A: ");
		uart_hex(n);
        uart_print("\r\n");
		task_sleep(RECV_DELAY);
	}
}

static void t2() {
    int n;
	while (1) {
		mbox_recv(&mb, &n, 1);
		uart_print(" B: ");
		uart_hex(n);
        uart_print("\r\n");
		task_sleep(RECV_DELAY);
	}
}

void setup(){
    uart_init();
    pic_init();
    timer_init();
    os_init();

    int i;
    for(i=0;i<30000000;++i)
        asm volatile("nop");

    mbox_init(&mb, buff, 5);

	task_create(t0, 0, 0);
	task_create(t1, 0, 0);
	task_create(t2, 0, 0);
    os_start();
}
