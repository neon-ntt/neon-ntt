
#include <stdio.h>
#include "hal.h"

void hal_send_str(const char*in){
    printf("%s\n", in);
}

uint64_t hal_get_time(void){
  uint64_t t;
  asm volatile("mrs %0, PMCCNTR_EL0":"=r"(t));
  return t;
}
