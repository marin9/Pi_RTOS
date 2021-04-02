#include "rpi.h"

#define PWM_CONTROL		((volatile uint*)(PWM_BASE + 0x00))
#define PWM_STATUS		((volatile uint*)(PWM_BASE + 0x04))
#define PWM0_RANGE		((volatile uint*)(PWM_BASE + 0x10))
#define PWM0_DATA		((volatile uint*)(PWM_BASE + 0x14))
#define PWM_FIFO    	((volatile uint*)(PWM_BASE + 0x18))
#define PWM1_RANGE		((volatile uint*)(PWM_BASE + 0x20))
#define PWM1_DATA		((volatile uint*)(PWM_BASE + 0x24))
#define PWMCLK_CNTL		((volatile uint*)(CLOCK_BASE + 0xA0))
#define PWMCLK_DIV		((volatile uint*)(CLOCK_BASE + 0xA4))

// Control reg
#define PWM0_ENABLE     0x0001  // Channel Enable
#define PWM0_SERIAL     0x0002  // Run in serial mode
#define PWM0_REPEATFF   0x0004  // Repeat last value if FIFO empty
#define PWM0_OFFSTATE   0x0008  // Ouput Off state
#define PWM0_REVPOLAR   0x0010  // Reverse polarity
#define PWM0_USEFIFO    0x0020  // Data from FIFO
#define PWM0_CLRFIFO    0x0040  // Clear FIFO
#define PWM0_MS_MODE    0x0080  // Run in MS mode
#define PWM1_ENABLE     0x0100  // Channel Enable
#define PWM1_SERIAL     0x0200  // Run in serial mode
#define PWM1_REPEATFF   0x0400  // Repeat last value if FIFO empty
#define PWM1_OFFSTATE   0x0800  // Ouput Off state
#define PWM1_REVPOLAR   0x1000  // Reverse polarity
#define PWM1_USEFIFO    0x2000  // Data from FIFO
#define PWM1_MS_MODE    0x8000  // Run in MS mode

// Status reg
#define FULL1 			0x1
#define EMPT1 			0x2
#define WERR1 			0x4
#define RERR1 			0x8
#define GAPO1 			0x10
#define GAPO2 			0x20
#define GAPO3 			0x40
#define GAPO4 			0x80
#define BERR  			0x100

#define PM_PASS 		0x5A000000


void pwm_init(uint div, uint range) {
    // div = 40000, range = 0x100
    gpio_mode(12, GPIO_FN0);

    *PWMCLK_CNTL = PM_PASS | (1 << 5);
    *PWMCLK_DIV = PM_PASS | div;
    *PWMCLK_CNTL = PM_PASS | 16 | 1;
    timer_delay(2000);

    *PWM_CONTROL = 0;
    timer_delay(2000);

    *PWM0_RANGE = range;
    *PWM_CONTROL = PWM0_USEFIFO | PWM0_ENABLE | (1 << 6);
    timer_delay(2000);
}

void pwm_deinit() {
    *PWM_CONTROL = 0;
}

uint pwm_write(char *buff, uint len) {
	uint i = 0;

    while (len && !(*PWM_STATUS & FULL1)) {
        *PWM_FIFO = (int)buff[i];
        ++i;
        --len;
    }
    return i;
}
