#include "tso_lib.h"
#include <stdio.h>

void* tso_alloc(size_t size) {
  char * aux = "";
  void** address;

  system("echo llamando syscall...");

  long res = syscall(SYSCALL_ALLOC, size, address);

  sprintf(aux, "echo address: %p \n",  address);
  system(aux);
  sprintf(aux, "echo *address %p \n", *address);
  system(aux);

  return res != 0 ? *address : NULL;
}

long tso_free(void* address) {

  long res = syscall(SYSCALL_FREE, address);

  return res != 0 ? 0 : -1;

}

int tso_switch_strategy(ALLOCATION_STRATEGY strategy) {

  long res = syscall(SYSCALL_SWITCH_STRATEGY);

  int ret = res != 0 ? 0 : -1;
  return ret;
}
