#pragma once
#include "types.h"

#define SYSTMR1_IRQ	1
#define SYSTMR3_IRQ 3
#define GPIO_IRQ_0  49
#define GPIO_IRQ_1  50
#define GPIO_IRQ_2  51
#define GPIO_IRQ_3  52
#define IRQ_COUNT	72

#define GP_LOW 			0
#define GP_HIGH 		1
#define GP_IN			0
#define GP_OUT			1
#define GP_FN0			4
#define GP_FN1			5
#define GP_FN2			6
#define GP_FN3			7
#define GP_FN4			3
#define GP_FN5			2
#define GP_PULLOFF		(0<<4)
#define GP_PULLDOWN		(1<<4)
#define GP_PULLUP		(2<<4)
#define GP_NO_EDGE      0
#define GP_RISING_EDGE	1
#define GP_FALLING_EDGE	2
#define GP_HIGH_EDGE	3
#define GP_LOW_EDGE		4


void timer_init();
void timer_clr();
uint timer_get();
void timer_set(uint us);
void timer_wait(uint us);

void pic_init();
void pic_enable(uint irqn);
void pic_disable(uint irqn);
uint pic_ispending(uint irqn);

void gpio_open(uint pin, int mode);
void gpio_write(uint pin, int val);
uint gpio_read(uint pin);
void gpio_enint(uint pin, void (*h)(), int edge);

void uart_init();
void uart_print(char *s);
int uart_read(int unit, void* buffer, int size);
int uart_write(int unit, void* buffer, int size);
