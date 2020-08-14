#pragma once

#define BIT_VAL(n) 			(1 << (n))
#define BIT_GET(x, n) 		(((x) >> (n)) & 1)
#define BIT_SET(x, n)		((x) | (1 << (n)))
#define BIT_CLR(x, n)		((x) & ( ~(1 << (n))))
#define BIT_INV(x, n)		((x) ^ (1 << (n)))
#define BIT_WR(x, n, v)		(((x) & ( ~(1 << (n)))) | (((v) & 1) << (n)))

#define DEC2ASCII(dec)		((dec) + 0x30)
#define ASCII2DEC(asc)		((asc) - 0x30)
#define HEX2ASCII(hex)  	(((hex) > 0x09) ? ((hex) + 0x37): ((hex) + 0x30)) 
#define ASCII2HEX(asc)  	(((asc) > 0x39) ? ((asc) - 0x37): ((asc) - 0x30))

#define GET_BYTE0(b)		((b >> 0) & 0xFF)
#define GET_BYTE1(b)		((b >> 8) & 0xFF)
#define GET_BYTE2(b)		((b >> 16) & 0xFF)
#define GET_BYTE3(b)		((b >> 24) & 0xFF)

#define GET_MAX(a, b)		(((a) > (b))? (a): (b))
#define GET_MIN(a, b)		(((a) < (b))? (a): (b))
#define GET_ABS(a)			(((a) < 0)? -(a): (a))


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
