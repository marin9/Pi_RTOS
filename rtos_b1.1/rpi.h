#pragma once

#define PERIPH_BASE		0x20000000
#define SYSTMR_BASE		(PERIPH_BASE + 0x003000)
#define IRQ_BASE		(PERIPH_BASE + 0x00B000)
#define GPIO_BASE		(PERIPH_BASE + 0x200000)
#define UART0_BASE		(PERIPH_BASE + 0x201000)

#define IRQ_TMR 		1
#define IRQ_GPIO0  		49
#define IRQ_UART		57
#define IRQ_COUNT		64

#define GPIO_IN			0
#define GPIO_OUT		1
#define GPIO_FN0		4

typedef unsigned int uint;

void pic_init();
void pic_enable(uint irq);
void pic_disable(uint irq);
uint pic_pending(uint irq);

void timer_init();
uint timer_get();
void timer_set(uint us);
void timer_delay(uint us);

void gpio_init(uint pin, int mode);
uint gpio_set(uint pin, int value);
uint gpio_get(uint pin);

void uart_init();
void uart_print(char *s);
void uart_hex(uint n);
char uart_getchar();
void uart_intren();
