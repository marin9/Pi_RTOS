#include "api.h"
#include "prog.h"


void test(){
	Printf("\n\n");
	Printf("TEST, insert number:\n");
	Printf("1 - test stdio\n");
	Printf("2 - test memory\n");
	Printf("3 - test time\n");
	Printf("4 - test task\n");
	Printf("5 - test sem\n");
	Printf("\n>");

	char c;
	while(!(c=Getc()));
	
	Printf("\n\n");
	switch(c){
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
			Printf("Illegal number.\n");
			break;
	}
}