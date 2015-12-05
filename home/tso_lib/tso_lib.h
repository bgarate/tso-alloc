#ifndef TSO_LIB
#define TSO_LIB

#include <unistd.h>
#include <stddef.h>

#define SYSCALL_ALLOC 360
#define SYSCALL_FREE 361
#define SYSCALL_SWITCH_STRATEGY 362

typedef enum {
  FIRST_FIT = 0,
  BEST_FIT = 1,
  WORST_FIT = 2
} ALLOCATION_STRATEGY;

void* tso_alloc(size_t size);
long tso_free(void* address);
int tso_switch_strategy(ALLOCATION_STRATEGY strategy);

#endif
