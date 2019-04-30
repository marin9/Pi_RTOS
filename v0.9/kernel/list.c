#include "list.h"
#include "err.h"

void list_init(list_t *l){
	ASSERT(l, "Null pointer.");
	l->count=0;
	l->first=0;
	l->last=0;
}

int list_count(list_t *l){
	ASSERT(l, "Null pointer.");
	return l->count;
}

void list_addFirst(list_t *l, node_t* n, void* obj){
	ASSERT(l && n && obj, "Null pointer.");
	++(l->count);
	n->object=obj;

	if(l->first==0){
		n->next=0L;
		n->prev=0;
		l->first=n;
		l->last=n;
	}else{
		n->next=l->first;
		n->prev=0;
		l->first->prev=n;
		l->first=n;
	}
}

void list_addLast(list_t *l, node_t* n, void* obj){
	ASSERT(l && n && obj, "Null pointer.");
	++(l->count);
	n->object=obj;

	if(l->first==0){
		n->next=0;
		n->prev=0;
		l->first=n;
		l->last=n;
	}else{
		n->next=0;
		n->prev=l->last;
		l->last->next=n;
		l->last=n;
	}
}

void list_addSort(list_t *l, node_t* n, void* obj, int (*fcmp)(void *o1, void *o2)){
	ASSERT(l && n && obj && fcmp, "Null pointer.");
	n->object=obj;

	node_t *current=l->first;
	if(current==0 || fcmp(current->object, obj)>0){
		list_addFirst(l, n, obj);
		return;
	}
	current=current->next;

	while(current!=0){
		if(fcmp(current->object, obj)>0){
			node_t *n1=current->prev;
			node_t *n2=current;
			current=n;

			n1->next=current;
			n2->prev=current;
			current->next=n2;
			current->prev=n1;
			++(l->count);
			return;
		}
		current=current->next;
	}
	list_addLast(l, n, obj);
}

node_t* list_removeFirst(list_t *l){
	ASSERT(l, "Null pointer.");
	if(l->first!=0){
		--(l->count);
		node_t *item=l->first;

		l->first=l->first->next;
		if(l->first==0){
			l->last=0;
		}else{
			l->first->prev=0;
		}
		return item;
	}else{
		return 0;
	}
}


node_t* list_remove(list_t *l, void *id, int (*fid)(void *o1, void *o2)){
	ASSERT(l && id && fid, "Null pointer.");
	node_t *tmp=list_getFirst(l);
	while(tmp){
		if(fid(id, tmp->object)){
			--(l->count);
			node_t *item;
			if(l->first==tmp){
				item=l->first;
				l->first=l->first->next;
				if(l->first==0){
					l->last=0;
				}else{
					l->first->prev=0;
				}
				return item;
			}else if(l->last==tmp){
				item=l->last;
				l->last=l->last->prev;
				if(l->last==0){
					l->first=0;
				}else{
					l->last->next=0;
				}
				return item;
			}else{
				item=tmp;
				node_t* nx=tmp->next;
				node_t* pr=tmp->prev;

				pr->next=nx;
				nx->prev=pr;
			}
			return item;
		}
		tmp=tmp->next;
	}
	return 0;
}

node_t* list_getFirst(list_t *l){
	ASSERT(l, "Null pointer.");
	if(l->first!=0){
		return l->first;
	}else{
		return 0;
	}
}

node_t* list_get(list_t *l, void *id, int (*fid)(void *o1, void *o2)){
	ASSERT(l && id && fid, "Null pointer.");
	node_t *tmp=list_getFirst(l);
	while(tmp){
		if(fid(id, tmp->object)) return tmp;
		tmp=tmp->next;
	}
	return 0;
}
