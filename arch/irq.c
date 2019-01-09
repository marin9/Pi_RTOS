#include "irq.h"
#include "err.h"

#define IRQ_COUNT 32
#define NVIC_BASE       (0x10140000)
#define VICIRQSTATUS    ((volatile unsigned int*)(NVIC_BASE+0x000))
#define VICFIQSTATUS    ((volatile unsigned int*)(NVIC_BASE+0x004))
#define VICRAWINTR      ((volatile unsigned int*)(NVIC_BASE+0x008))
#define VICINTSELECT    ((volatile unsigned int*)(NVIC_BASE+0x00C))
#define VICINTENABLE    ((volatile unsigned int*)(NVIC_BASE+0x010))
#define VICINTENCLEAR   ((volatile unsigned int*)(NVIC_BASE+0x014))
#define VICSOFTINT      ((volatile unsigned int*)(NVIC_BASE+0x018))


static void (*irq_handlers[IRQ_COUNT])();
static void (*irq_clearers[IRQ_COUNT])();


void irq_init(){
  int i;
  for(i=0;i<IRQ_COUNT;++i){
    irq_handlers[i]=0;
    irq_clearers[i]=0;
  }
  *VICINTENABLE=0;
}

void irq_register(int irq_num, void (*handler)(), void (*clearer)()){
  ASSERT(irq_num>=0 && irq_num<32, "Illegal irq number.");
  irq_handlers[irq_num]=handler;
  irq_clearers[irq_num]=clearer;
  *VICINTENABLE=*VICINTENABLE|(1<<irq_num);
}

void irq_unregister(int irq_num){
  ASSERT(irq_num>=0 && irq_num<32, "Illegal irq number.");
  irq_handlers[irq_num]=0;
  irq_clearers[irq_num]=0;
  *VICINTENCLEAR=1<<irq_num;
}


void interrupt_handler(){
  int i;
  for(i=0;i<IRQ_COUNT;++i){
    if(*VICIRQSTATUS&(1<<i)){
      if(irq_clearers[i]) irq_clearers[i]();
      if(irq_handlers[i]) irq_handlers[i]();
    }
  }
}

void undefined_instruction(){
  ASSERT(0, "Undefined instruction.");
}
