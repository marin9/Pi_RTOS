#include "string.h"
#include "err.h"


void *memset(void *s, int c, int n){
	ASSERT(s, "Null pointer");
	int p;
	char *m=(char*)s;

	for(p=0;p<n;p++,m++) *m=(char)c;
	return s;
}

void *memcpy(void *dest, void *src, int n){
	ASSERT(dest && src, "Null pointer");
	int p;
	char *d=(char*)dest;
	char *s=(char*)src;

	for(p=0;p<n; p++, d++, s++) *d=*s;
	return dest;
}


int strlen(char *s){
	ASSERT(s, "Null pointer");
	int i;
	for(i=0;s[i];i++);
	return i;
}


void itoa(char *buffer, int base, int d){
	ASSERT(buffer, "Null pointer");

	char *p1, *p2, firsthexchar, *p=buffer;
	unsigned long ud = d;
	int divisor=10;
	int digits=0;

	/* if number is negative and format is '%d', insert starting '-' */
	if(base=='d' && d<0){
		*p++='-';
		 buffer++;
		ud= -d;
	}else if(base=='x' || base=='X'){
		divisor=16;
	}
	firsthexchar=(base=='x' ? 'a' : 'A');

	/* divide 'ud' with base 'divisor' until 'ud' is not 0 */
	do{
		int remainder=ud%divisor;

		if(remainder < 10){
			*p++=remainder + '0';
		}else{
			*p++=remainder+firsthexchar-10;
		}
		digits++;
		ud=ud/divisor;
	}while(ud);
	/* add leading zeros if hexadecimal format is required */
	if(base=='x' || base=='X'){
		while(digits<8){
			digits++;
			*p++='0';
		}
		*p++='x';
		*p++='0';
	}
	/* Add terminating character */
	*p=0;

	/* Reverse string */
	p1=buffer;
	p2=p-1;
	while(p1<p2){
		char tmp=*p1;
		*p1=*p2;
		*p2=tmp;
		p1++;
		p2--;
	}
}


/*! Formated output to console (lightweight version of 'printf') */
int vssprintf(char *str, int size, char **arg){
	ASSERT(str && arg, "Null pointer");

	char *format=*arg, buffer[20], *p;
	int c, i=0;

	if(!format) return 0;
	arg++; /* first argument after 'format' (on stack) */

	while((c=*format++)!=0 && i<size-1){
		if(c!='%'){
			str[i++]=(char)c;
		}else{
			c=*format++;
			switch(c){
			case 'd':
			case 'u':
			case 'x':
			case 'X':
				itoa(buffer, c, *((int*)arg++));
				p=buffer;
				if(i+strlen(p)<size-1)
					while(*p)
						str[i++]=*p++;
				else
					goto too_long;
				break;
			case 's':
				p=*arg++;
				if(!p) p="(null)";
				if(i+strlen(p)<size-1)
					while(*p) str[i++]=*p++;
				else
					goto too_long;
				break;
			default: /* assuming c=='c' */
				str[i++]=*((int*)arg++);
				break;
			}
		}
	}
too_long: /* just print what did fit */
	str[i++]=0;
	return i;
}
