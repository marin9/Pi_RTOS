#include "rpi.h"
#include "dev.h"
#include "lib.h"
#include "os.h"


void putc(char c) {
	uart_write(&c, 1, 1);
}

void print(char *s) {
	uart_write(s, strlen(s), 1);
}


void kb_int() {
	char k = 0, s = 0, c = 0;

	kb_read(&k, &s, &c);

	if (k != 0 && s == 1)
		putc(k);
}

void setup() {
    pic_init();
    timer_init();
    uart_init(115200);
    i2c_init(400000);
    spi_init(100000);
    pwm_init(40000, 0x100);
	kb_init();
    os_init();

    // User code when using OS
	kb_setbacklight(100);
	gpio_mode(21, GPIO_IN);
	gpio_attachintr(21, kb_int, GPIO_FLEDG);
	pic_register(IRQ_GPIO0, gpio_irq_handler);
	pic_enable(IRQ_GPIO0);
	pic_unblock();

    // Start OS scheduer, otherwise start loop
    //os_start();
}

void loop() {
    // User code when OS is unused
}
