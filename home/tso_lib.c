#include "tso_lib.h"

void* tso_alloc(size_t size) {

  void* address;

  long res = syscall(SYSCALL_ALLOC, size, address);

  return res != 0 ? address : NULL;
}

long tso_free(void* address) {

  long res = syscall(SYSCALL_FREE, address);

  return res != 0 ? 0 : -1;

}

void tso_switch_strategy(ALLOCATION_STRATEGY strategy) {

  long res = syscall(SYSCALL_SWITCH_STRATEGY);

  return res != 0 ? 0 : -1;

}
