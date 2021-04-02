#include "rpi.h"
#include "dev.h"
#include "lib.h"
#include "os.h"


void setup() {
    pic_init();
    timer_init();
    uart_init(115200);
    i2c_init(400000);
    spi_init(100000);
    pwm_init(40000, 0x100);
    os_init();

    // User code when using OS

    // Start OS scheduer, otherwise start loop
    //os_start();
}

void loop() {
    // User code when OS is unused
}
