#pragma once
#include "types.h"

typedef struct{
	void (*init)();
	int (*get)();
	void (*enable)(uint irq);
	void (*disable)(uint irq);
} irqcontroller_t;


typedef struct{
	uint dev_id;
	uint irq_num;

	void (*init)();
	void (*clri)();
	uint (*get)();
	void (*set)(uint us);
	void (*destroy)();
} timer_t;


typedef struct{
	uint dev_id;
	uint irq_num;
	uint flags;
	void *param;

	int (*init)(uint param);	
	int (*stat)(uint param);
	int (*send)(void *data, uint size);
	int (*recv)(void *data, uint size);
	void (*destroy)();
	void (*clri)(); 
} device_t;
