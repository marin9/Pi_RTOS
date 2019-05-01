#include "lib.h"


void queue_init(queue_t *q){
	q->count=0;
	q->first=0;
	q->last=0;
}

int queue_count(queue_t *q){
	return q->count;
}

void* queue_peek(queue_t *q){
	return q->first->object;
}

void* queue_next(node_t *n){
	return n->next->object;
}

void queue_push(queue_t *q, node_t *n, void *o){
	n->object=o;
	++(q->count);

	if(q->first==0){
		n->next=0;
		n->prev=0;
		q->first=n;
		q->last=n;
	}else{
		n->next=0;
		n->prev=q->last;
		q->last->next=n;
		q->last=n;
	}
}

void* queue_pop(queue_t *q){
	if(q->first==0){
		return 0;
	}

	--(q->count);
	node_t *n=q->first;

	q->first=q->first->next;
	if(q->first==0){
		q->last=0;
	}else{
		q->first->prev=0;
	}
	return n->object;
}

void* queue_remove(queue_t *q, node_t *n){
	if(q->first==n){
		q->first=n->next;
		q->first->prev=0;
	}else if(q->last==n){
		q->last=q->last->prev;
		q->last->next=0;
	}else{
		n->prev->next=n->next;
		n->next->prev=n->prev;
	}
	n->next=0;
	n->prev=0;
	return n->object;
}
