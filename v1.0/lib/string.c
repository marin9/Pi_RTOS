#include "types.h"


void *memset(void *s, int c, uint n){
	uint p;
	char *m=(char*)s;
	for(p=0;p<n;p++,m++){
		*m=(char)c;
	}
	return s;
}

void *memcpy(void *dest, void *src, uint n){
	uint p=0;
	char *d=(char*)dest;
	char *s=(char*)src;

	while(p<n){
		*d++=*s++;
		++p;
	}
	return dest;
}

uint strlen(char *s){
	uint i;
	for(i=0;s[i];i++);
	return i;
}

int strcmp(const char *s1, const char *s2){
	int i;
	for(i=0;s1[i] || s2[i];i++){
		if(s1[i]<s2[i]) return -1;
		else if(s1[i]>s2[i]) return 1;
	}
	return 0;
}
