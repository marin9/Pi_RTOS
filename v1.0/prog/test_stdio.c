#include "api.h"
#include "string.h"

void test_stdio(){
	int n;
	char buffer[16];
	Printf("TEST STDIO\r\n");

	while(1){
		memset(buffer, 0, 16);
		n=Gets(buffer, 15);
		Printf("[%s][%d]\r\n", buffer, n);
	}
}