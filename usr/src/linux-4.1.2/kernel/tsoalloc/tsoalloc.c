#include <linux/tsoalloc.h>
#include <linux/list.h>

#define START_NEXT_FREE(dsd)
#define SIZE_NEXT_FREE(asdasd)


static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;


asmlinkage long sys_tso_mm_alloc(size_t size) {
  return 0;
}

asmlinkage long sys_tso_mm_free(void* addr) {
  return 0;
}

asmlinkage long sys_tso_mm_switch_strategy(enum ALLOCATION_STRATEGY strategy) {
  return 0;
}

void __tso_mm_initialize() {

}

void __tso_mm_expand() {

}

long __tso_mm_get_fit(size_t size) {
  switch(current_strategy) {
  case WORST_FIT:
    return __tso_mm_get_worst_fit(size);
    break;
  case BEST_FIT:
    return __tso_mm_get_best_fit(size);
    break;
  case FIRST_FIT:
    return __tso_mm_get_first_fit(size);
    break;
  }
}

long __tso_mm_get_best_fit(size_t size) {
  return 0;
}

long __tso_mm_get_worse_fit(size_t size) {
  return 0;
}

long __tso_mm_get_first_fit(size_t size) {
  return 0;
}

void __tso_mm_add_region(struct tso_mm_region region) {

}

void __tso_mm_remove_region(struct tso_mm_region region) {

}

void __tso_mm_split_region(struct tso_mm_region previous, struct tso_mm_region freed_region, struct tso_mm_region next) {

}

void __tso_mm_join_region(struct tso_mm_region previous, struct tso_mm_region freed_region, struct tso_mm_region next) {

}
