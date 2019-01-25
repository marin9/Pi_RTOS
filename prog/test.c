#include "api.h"
#include "prog.h"


void test(){
	char buffer[4];

	Printf("\r\n\r\n");
	Printf("TEST, insert number:\r\n");
	Printf("1 - test stdio\r\n");
	Printf("2 - test memory\r\n");
	Printf("3 - test time\r\n");
	Printf("4 - test task\r\n");
	Printf("5 - test sem\r\n");
	Printf("\r\n>");

	Gets(buffer, 3);
	Printf("\r\n");

	switch(buffer[0]){
		case '1':
			test_stdio();
			break;
		case '2':
			test_memory();
			break;
		case '3':
			test_time();
			break;
		case '4':
			test_task();
			break;
		case '5':
			test_sem();
			break;
		default:
			Printf("Illegal number.\r\n");
			break;
	}
}