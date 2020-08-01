#include "rpi.h"

#define RNG_CTRL        ((volatile uint*)(RAND_BASE+0x00))
#define RNG_STATUS      ((volatile uint*)(RAND_BASE+0x04))
#define RNG_DATA        ((volatile uint*)(RAND_BASE+0x08))
#define RNG_INT_MASK	((volatile uint*)(RAND_BASE+0x10))


void rand_init() {
   *RNG_STATUS = 0x40000;
    // mask interrupt
    *RNG_INT_MASK |= 1;
    // enable
    *RNG_CTRL |= 1;
    // wait for gaining some entropy
    while (!((*RNG_STATUS) >> 24));
}

int rand_get(int min, int max) {
	return *RNG_DATA % (max - min) + min;
}
