#pragma once

#define	NULL	((void*)0)

void *memset(void *s, int c, int n);
void *memcpy(void *dest, void *src, int n);
int strlen(char *s);
void itoa(char *buffer, int base, int d);
int vssprintf(char *str, int size, char **arg);
