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

void setup() {
    pic_init();
    timer_init();
    uart_init(115200);
    i2c_init(100000); // 1 --> 4 ?
    spi_init(100000);
    pwm_init(40000, 0x100);
    os_init();

    // User code when using OS
kb_init();
    // Start OS scheduer, otherwise start loop
    //os_start();
}

void loop() {
    // User code when OS is unused
	char k = 0, s = 0, c = 0;
	kb_read(&k, &s, &c);

	putc(k);
	putc('\r');
	putc('\n');
	if (c & (1 << 5)) print(" caps lock\r\n");
	if (c & (1 << 6)) print(" num lock\r\n");
	if (k != 0) print(" not zero\r\n");

	timer_delay(100000);
	if (k == '0') kb_setbacklight(0);
	if (k == '1') kb_setbacklight(50);
	if (k == '2') kb_setbacklight(100);
	if (k == '3') kb_setbacklight(150);
	if (k == '4') kb_setbacklight(200);
	if (k == '5') kb_setbacklight(255);
	timer_delay(100000);
}
