#include "api.h"

void test_memory(){
  // mem.c uncomment for test
  Printf("TEST MEMORY\n");

  void *m1=Malloc(100);
  void *m2=Malloc(100);
  void *m3=Malloc(100);
  void *m4=Malloc(100);
  void *m5=Malloc(100);

  Printf("m1 addr:\t\t%x\n", (unsigned int)m1);
  Printf("m2 addr:\t\t%x\n", (unsigned int)m2);
  Printf("m3 addr:\t\t%x\n", (unsigned int)m3);
  Printf("m4 addr:\t\t%x\n", (unsigned int)m4);
  Printf("m5 addr:\t\t%x\n", (unsigned int)m5);

  Free(m2);
  Free(m4);
  Free(m3);
  Free(m1);

  Printf("\n");

  m1=Malloc(100);
  m2=Malloc(100);
  m3=Malloc(100);
  m4=Malloc(100);
  m5=Malloc(100);

  Printf("m1 addr:\t\t%x\n", (unsigned int)m1);
  Printf("m2 addr:\t\t%x\n", (unsigned int)m2);
  Printf("m3 addr:\t\t%x\n", (unsigned int)m3);
  Printf("m4 addr:\t\t%x\n", (unsigned int)m4);
  Printf("m5 addr:\t\t%x\n", (unsigned int)m5);

  Free(m2);
  Free(m4);
  Free(m3);
  Free(m1);

  while(1);
}