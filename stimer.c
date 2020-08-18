#include "rpi.h"
#include "dev.h"


typedef struct {
	uint val;
	uint rel;
	uint run;
	uint per;
	void (*func)();
} tmr_t;

static tmr_t timers[TMRS_COUNT];

void tmr_tick() {
	int i;
	for (i = 0; i < TMRS_COUNT; ++i) {
		if (timers[i].run) {
			if (timers[i].val) {
				timers[i].val -= 1;
			} else {
				if (timers[i].func)
					timers[i].func();

				if (timers[i].per) {
					timers[i].val = timers[i].rel;
				} else {
					timers[i].run = 0;
				}
			}
		}
	}
	timer_set(1000);
}

void tmr_init(uint id, uint inval, uint periodic) {
	timers[id].val = inval;
	timers[id].rel = inval;
	timers[id].per = periodic;
	timers[id].run = 0;
	timers[id].func = 0;
}

void tmr_start(uint id) {
	timers[id].run = 1;
}

void tmr_stop(uint id) {
	timers[id].run = 0;
}

void tmr_set(uint id, uint inval) {
	timers[id].val = inval;
	timers[id].rel = inval;
}

uint tmr_get(uint id) {
	return timers[id].val;
}

void tmr_attachintr(uint id, void (*func)()) {
	timers[id].func = func;
}

