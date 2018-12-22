#include "api.h"


void test_stdio(){
	int c;
	Printf("TEST STDIO\n");
	while(1){
		c=Getc();
		if(c) Putc(c);
	}
}