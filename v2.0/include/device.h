#pragma once

typedef struct{
	void (*irq_init)();
	int (*irq_get)();
	void (*irq_enable)(uint irq);
	void (*irq_disable)(uint irq);
} irqcontroller_t;


typedef struct{
	uint dev_id;
	uint irq_num;
	void (*timer_init)();
	void (*timer_clri)();
	uint (*timer_get)();
	void (*timer_set)(uint us);
	void (*timer_destroy)();
} timer_t;


typedef struct{



} device_t;

