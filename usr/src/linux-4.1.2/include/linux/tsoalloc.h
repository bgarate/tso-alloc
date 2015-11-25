#ifndef TSO_ALLOC
#define TSO_ALLOC

#include <linux/list.h>

#define INITIAL_SIZE 8
#define MAX_SIZE 1024

typedef unsigned long addr_t;
typedef unsigned int size_t;
  
enum ALLOCATION_STRATEGY {
  FIRST_FIT = 0,
  BEST_FIT = 1,
  WORST_FIT = 2
};

struct tso_mm_region {
  tso_mm_region* next;
  size_t size;
  size_t free;
};

struct tso_mm_mapping {
<<<<<<< HEAD
  tso_mm_region* first_region;
  void* start;
=======
  struct tso_mm_region* used;
  struct tso_mm_region* free;
  addr_t start;
>>>>>>> origin/master
  size_t size;
};

asmlinkage long sys_tso_mm_alloc(size_t size);
asmlinkage long sys_tso_mm_free(void* addr);
asmlinkage long sys_tso_mm_switch_strategy(enum ALLOCATION_STRATEGY strategy);

#endif
