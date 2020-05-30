#include "rpi.h"
#include "lib.h"
#include "os.h"

static char buff[128];


static void delay(uint us) {
    us += timer_get();
    while (timer_get() < us);
}


void setup() {
    // user code
    pic_init();
    uart_init();
    timer_init();
    spi_init(32);

    delay(4000000);

    flash_read(0, buff, 128);
    uart_print(buff);
}
