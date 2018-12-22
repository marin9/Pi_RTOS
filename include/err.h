#pragma once
#include "cpu.h"
#include "stdio.h"


#define ASSERT(expr, str)                 \
  do if(!(expr)){                         \
    printf("%sERROR: %s:%d: %s%s\n",      \
    RED, __FILE__, __LINE__, str, WHITE); \
    cpu_halt();                           \
  }while(0)
