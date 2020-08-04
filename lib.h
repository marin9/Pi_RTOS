#pragma once

#define BIT_VAL(n) 			(1<<n)
#define BIT_GET(x, n) 		((x>>n)&1)
#define BIT_SET(x, n)		(x|(1<<n))
#define BIT_CLR(x, n)		(x&(~(1<<n)))
#define BIT_INV(x, n)		(x^(1<<n))
#define BIT_WR(x, n, v)		((x&(~(1<<n)))|((v&1)<<n))


void *memset(void *s, int c, int n);
void *memcpy(void *dest, void *src, int n);
int strcmp(const char *s1, const char *s2);
int strlen(const char *s);

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);
