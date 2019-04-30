#include "mem.h"
#include "cpu.h"
#include "err.h"

#define CHUNK_SIZE  4096

struct mem_hdr{
  void *first;
  void *last;
};

static struct mem_hdr mhdr;
static unsigned int free_chunks;

extern unsigned int heap_start;
extern unsigned int __end;

static unsigned int heap_addr;
static unsigned int heap_size;
static int chunk_num;


// mask for align
static unsigned int get_mask(unsigned int block){
  unsigned int mask=0;
  unsigned int pot=((unsigned int)(-1))&0x80000000;
  while(pot){
    if(pot>=block) mask=mask|pot;
    pot=pot/2;
  }
  return mask;
}


void mem_init(){
  char *heap_begin=(char*)&heap_start;
  char *heap_end=(char*)&__end;
  unsigned int mask=get_mask(CHUNK_SIZE);
  heap_addr=((unsigned int)heap_begin+CHUNK_SIZE)&mask;
  heap_size=((char*)heap_end-heap_begin)&mask;
  chunk_num=heap_size/CHUNK_SIZE;


  //printf("HEAP START: %x\n", heap_addr);
  //printf("RAM: %u B\n", heap_size);
  //printf("CHUNK NUM: %u\n", chunk_num);
 	//chunk_num=5;	//for testing

  mhdr.first=(void*)heap_addr;
  mhdr.last=(void*)(heap_addr+CHUNK_SIZE*(chunk_num-1));

  int i;
  void *p=mhdr.first;

  for(i=0;i<chunk_num;++i){
    *((unsigned int*)p)=(unsigned int)((unsigned int)p+CHUNK_SIZE);
    p=p+CHUNK_SIZE;
  }
  free_chunks=chunk_num-1;
}

void* mem_alloc(int size){
  if(free_chunks==0 || size<0 || size>CHUNK_SIZE) return 0;
  --free_chunks;

  void* chunk=mhdr.first;
  mhdr.first=(void*)(*((unsigned int*)mhdr.first));
  return chunk;
}

void mem_free(void *p){
  ASSERT(p, "Null pointer.");
  ++free_chunks;

  *((unsigned int*)(mhdr.last))=(unsigned int)p;
  mhdr.last=p;
}