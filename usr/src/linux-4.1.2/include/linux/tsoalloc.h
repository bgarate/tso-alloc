#ifndef TSO_ALLOC
#define TSO_ALLOC

#define INITIAL_SIZE 8
#define MAX_SIZE 1024

typedef unsigned int size_t;
  
enum ALLOCATION_STRATEGY {
  FIRST_FIT = 0,
  BEST_FIT = 1,
  WORST_FIT = 2
};

struct tso_mm_region {
  struct tso_mm_region* next;
  size_t size;
};

struct tso_mm_mapping {
  struct tso_mm_region* first_region;
  void* start;
  size_t size;
};

asmlinkage long sys_tso_mm_alloc(size_t size, void* ret);
asmlinkage long sys_tso_mm_free(void* addr);
asmlinkage long sys_tso_mm_switch_strategy(enum ALLOCATION_STRATEGY strategy);

#endif
