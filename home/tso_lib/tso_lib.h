#IFNDEF TSO_LIB
#DEFINE TSO_LIB

#include <unistd.h>
#include <stdef.h>

#DEFINE SYSCALL_ALLOC 360
#DEFINE SYSCALL_FREE 361
#DEFINE SYSCALL_SWITCH_STRATEGY 362

typedef enum {
  FIRST_FIT = 0,
  BEST_FIT = 1,
  WORST_FIT = 2
} ALLOCATION_STRATEGY;

void* tso_alloc(size_t size);
long tso_free(void* address);
void tso_switch_strategy(ALLOCATION_STRATEGY strategy);

#ENDIF
