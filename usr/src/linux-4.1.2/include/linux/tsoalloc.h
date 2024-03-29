#ifndef TSO_ALLOC
#define TSO_ALLOC

#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/linkage.h>

#define INITIAL_SIZE 8 * PAGE_SIZE
#define MAX_SIZE 1024 * PAGE_SIZE
  
typedef enum {
  FIRST_FIT = 0,
  BEST_FIT = 1,
  WORST_FIT = 2
} ALLOCATION_STRATEGY;

struct tso_mm_region {
  struct tso_mm_region* next;
  unsigned long size;
};

struct tso_mm_mapping {
  struct tso_mm_region* first_region;
  void* start;
  unsigned long size;
  unsigned long free;
};

asmlinkage long sys_tso_mm_alloc(size_t size, void** ret);
asmlinkage long sys_tso_mm_free(void* addr);
asmlinkage long sys_tso_mm_switch_strategy(unsigned int strategy);

#endif
