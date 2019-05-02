#include "types.h"

static char *mm_begin;
static uint n_blocks;
static uint b_size;


void memory_init(void *addr, uint bsize, uint n){
	uint i;
	mm_begin=(char*)addr;
	b_size=bsize;
	n_blocks=n;

	for(i=0;i<n_blocks;++i){
		*(mm_begin+b_size*i)=0;
	}
}


void* memory_alloc(uint size){
	uint i;
	if(size>b_size || !size){
		return 0;
	}
	for(i=0;i<n_blocks;++i){
		if(!(*(mm_begin+b_size*i))){
			*(mm_begin+b_size*i)=1;
			return mm_begin+b_size*i+1;
		}
	}
	return 0;
}

void memory_free(void *p){
	char *block=(char*)p;
	*(block-1)=0;
}
