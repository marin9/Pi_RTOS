#include "device.h"



void interrupt_handler(){

	uart_print("\tINTERRUPT\r\n");
	timer_ack();
}

