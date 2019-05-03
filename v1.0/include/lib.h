#pragma once
#include "types.h"

// Queue
void queue_init(queue_t *q);
int queue_count(queue_t *q);
void* queue_peek(queue_t *q);
void* queue_next(node_t *n);
void* queue_pop(queue_t *q);
void queue_push(queue_t *q, node_t *n, void *o);
void* queue_remove(queue_t *q, node_t *n);

// Strings
void *memset(void *s, int c, uint n);
void *memcpy(void *dest, void *src, uint n);
uint strlen(char *s);
int strcmp(const char *s1, const char *s2);
