#include "tso_lib.h"
#include <stdio.h>
#include <stdlib.h>

char aux[500];

void* tso_alloc(size_t size) {
  void* address = NULL;

  long res = syscall(SYSCALL_ALLOC, size, &address);

  //sprintf(aux, "echo address: %p \n",  address);
  //system(aux);

  return address;
  //return NULL;
}

long tso_free(void* address) {

  long res = syscall(SYSCALL_FREE, address);
  //sprintf(aux, "echo res free: %ld \n",  res);
  //system(aux);  
  return res != 0 ? 0 : -1;

}

int tso_switch_strategy(ALLOCATION_STRATEGY strategy) {

  printf("strategy before callback: %u \n", strategy);

  long res = syscall(SYSCALL_SWITCH_STRATEGY, strategy);

  printf("switch res: %ld \n", res);
  int ret = res != 0 ? 0 : -1;
  return ret;
}
