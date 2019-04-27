#pragma once

void uart_init();
int uart_recv(int *c);
int uart_send(int c);
