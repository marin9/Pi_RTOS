#pragma once

typedef struct node_t{
	void* object;
	struct node_t* next;
	struct node_t* prev;
} node_t;

typedef struct list_t{
	int count;
	node_t *first;
	node_t *last;
} list_t;


void list_init(list_t *l);
int list_count(list_t *l);
void list_addFirst(list_t *l, node_t* n, void* obj);
void list_addLast(list_t *l, node_t* n, void* obj);
void list_addSort(list_t *l, node_t* n, void* obj, int (*fcmp)(void *o1, void *o2));
node_t* list_removeFirst(list_t *l);
node_t* list_remove(list_t *l, void *id, int (*fid)(void *o1, void *o2));
node_t* list_getFirst(list_t *l);
node_t* list_get(list_t *l, void *id, int (*fid)(void *o1, void *o2));
