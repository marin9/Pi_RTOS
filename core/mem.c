#include "types.h"
#include "param.h"

static block_t *heap;


void memory_init(void *addr){
	int i;
	heap=(block_t*)addr;

	for(i=0;i<HEAP_NBLOCKS;++i){
		heap[i].used=0;
	}
}


void* memory_alloc(uint size){
	int i;

	if(!size || size>(HEAP_BLOCK-1)){
		return 0;
	}

	for(i=0;i<HEAP_NBLOCKS;++i){
		if(heap[i].used==0){
			heap[i].used=1;
			return heap[i].block;
		}
	}
	return 0;
}

void memory_free(void *p){
	block_t *block;
	if(p){
		block=(block_t*)(((char*)p)-sizeof(char));
		block->used=0;
	}
}
