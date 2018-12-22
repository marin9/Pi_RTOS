#include "stdio.h"
#include "uart.h"
#include "lib.h"
#include "err.h"


void stdio_init(){
  uart_init();
}

int getc(){
  return uart_recv();
}

int putc(char c){
  if(c==127){
  	uart_send('\b');
  	uart_send(' ');
  	uart_send('\b');
  }else if(c=='\n' || c=='\r'){
  	uart_send('\r');
  	uart_send('\n');
	}else{
  	uart_send(c);
	}
  return 1;
}

int puts(char *s){
  int n=0;
	while(*s!='\0'){
  		n += putc(*s++);
	}
  return n;
}

int printf(char *format, ...){
  char buffer[256];
  ASSERT(format, "Null pointer.");
  vssprintf(buffer, 256, &format);
  return puts(buffer);
}
