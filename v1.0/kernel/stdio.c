#include "stdio.h"
#include "uart.h"
#include "string.h"
#include "task.h"
#include "cpu.h"
#include "err.h"


void stdio_init(){
  uart_init();
}

int getc(char *c){
  int d;
  disable_interrupts();
  while(!uart_recv(&d)){
    task_sleep(task_self(), 1);
  }
  enable_interrupts();
  *c=(char)d;
  return 1;
}

int putc(char c){
  int d=(int)c;
  disable_interrupts();
  while(!uart_send(d)){
    task_sleep(task_self(), 1);
  }
  enable_interrupts();
  return 1;
}

int gets(char *s, int n){
  int i;
  char c;
  ASSERT(s, "Null pointer.");

  i=0;
  while(i<n){
    getc(&c);

    if(c==127){
      if(i>0){
        --i;
        puts("\b \b");
      }
      continue;
    }

    s[i]=c;
    putc(c);
    if(c=='\r'){
      break;
    }
    ++i;
  }
  s[i]=0;
  puts("\r\n");
  return i;
}

int puts(char *s){
  int n=0;
	while(*s){
  		n+=putc(*s);
      ++s;
	}
  return n;
}

int printf(char *format, ...){
  char buffer[128];
  ASSERT(format, "Null pointer.");
  vssprintf(buffer, 128, &format);
  return puts(buffer);
}
