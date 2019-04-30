#pragma once
#include "types.h"

// Timer0
void timer_init();
void timer_destroy();
void timer_ack();
uint timer_get();
void timer_set(uint us);

// Interrupt controller
void irq_init();
int irq_get();
void irq_enable(uint irq);
void irq_disable(uint irq);

// Uart
void uart_init();
int uart_send(int c);
int uart_recv(int *c);
void uart_putc(int c);
void uart_print(char *s);
void uart_printhex(uint x);
