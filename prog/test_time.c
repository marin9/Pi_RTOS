#include "api.h"


void test_time(){
  time_t to, tn;
  to.sec=to.usec=0;

  Printf("TEST TIME\n");
  while(1){
    int i;
    for(i=0;i<100;++i) asm volatile("");

    TimeGet(&tn);
    if(tn.sec!=to.sec){
      Printf("%d.%d\n", tn.sec, tn.usec);
    }

    to=tn;
    if(tn.sec==10){
      to.sec=to.usec=0;
      TimeSet(&to);
    }
  }
}
