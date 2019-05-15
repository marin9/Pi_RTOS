#include "dev.h"
#include "lib.h"

void test_task();
void test_memory();
void test_lock1();
void test_lock2();


void main(){
	char buff[2];

	memset(buff, 0, 2);

	uart_print("Select test:\r\n");
	uart_print("\ta : test memory\r\n");
	uart_print("\tb : test tasks\r\n");
	uart_print("\tc : test bin sem\r\n");
	uart_print("\td : test count sem\r\n");
	uart_print(">");

	uart_gets(buff, 1);

	switch(buff[0]){
		case 'a':
			test_memory();
			break;
		case 'b':
			test_task();
			break;
		case 'c':
			test_lock1();
			break;
		case 'd':
			test_lock2();
			break;
		default:
			uart_print("WARNING: Illegal command.\r\n");
	}
}
