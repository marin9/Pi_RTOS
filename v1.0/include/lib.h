#pragma once
#include "types.h"

// Memory Heap
void mm_init(uint addr, uint bsize, uint n);
void* mm_alloc(uint size);
void mm_free(void *p);

// Strings
void *memset(void *s, int c, uint n);
void *memcpy(void *dest, void *src, uint n);
uint strlen(char *s);
int strcmp(const char *s1, const char *s2);
