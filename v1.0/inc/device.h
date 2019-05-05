#pragma once
#include "types.h"

#define CPSR_IRQ_EN		0x80

#define CPSR_MODE_FIQ	0x11
#define CPSR_MODE_IRQ	0x12
#define CPSR_MODE_SVC	0x13
#define CPSR_MODE_ABT	0x17
#define CPSR_MODE_UND	0x1b
#define CPSR_MODE_SYS	0x1f

#define IRQ_WATCHDOG 	0	/* Watchdog timer */
#define IRQ_SWI			1	/* Software interrupt (not one generated with SVC/SWI) */
#define IRQ_COM_RX		2	/* Debug communications receive interrupt */
#define IRQ_COM_TX		3	/* Debug communications transmit interrupt */
#define IRQ_TIMER01		4	/* Timer 0 or 1 Timers on development chip */
#define IRQ_TIMER23		5	/* Timer 2 or 3 Timers on development chip */
#define IRQ_GPIO0		6	/* GPIO controller in development chip */
#define IRQ_GPIO1		7	/* GPIO controller in development chip */
#define IRQ_GPIO2		8	/* GPIO controller in development chip */
#define IRQ_GPIO3		9	/* GPIO controller in development chip */
#define IRQ_RTC			10	/* Real time clock in development chip */
#define IRQ_SSP			11	/* Synchronous serial port in development chip */
#define IRQ_UART0IRQL	12	/* UART0 on development chip */
#define IRQ_UART1IRQL	13	/* UART1 on development chip */
#define IRQ_UART2IRQL	14	/* UART2 on development chip */
#define IRQ_SCI0		15	/* Smart Card interface in development chip */
#define IRQ_CLCD		16	/* CLCD controller in development chip */
#define IRQ_DMA			17	/* DMA controller in development chip */
#define IRQ_PWRFAIL		18	/* Power failure from FPGA */
#define IRQ_MBX			19	/* Graphics processor on development chip */


// Processor
void cpu_lidt();
void cpu_sti();
void cpu_cli();
void context_switch(task_t *old, task_t *new);

// Interrupt controller
void irq_init();
int irq_get();
void irq_enable(uint irq);
void irq_disable(uint irq);

// Timer0
void timer_init();
void timer_destroy();
void timer_ack();
uint timer_get();
void timer_set(uint us);

// Uart
void uart_init();
int uart_send(int c);
int uart_recv(int *c);
void uart_putc(int c);
uint uart_getc();
void uart_print(char *s);
void uart_printhex(uint x);
uint uart_gets(char *b, uint n);
