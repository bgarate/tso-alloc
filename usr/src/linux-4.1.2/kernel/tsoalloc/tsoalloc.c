#include tsoalloc.h

#include <linux/list.h>

#define START_NEXT_FREE(dsd)
#define SIZE_NEXT_FREE(asdasd)


static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;


asmlinkage long sys_tso_mm_alloc(size_t size) {

}

asmlinkage long sys_tso_mm_free(void* addr) {

}

asmlinkage long sys_tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {

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
}

long __tso_mm_get_worse_fit(size_t size) {
}

long __tso_mm_get_first_fit(size_t size) {
}

void __tso_mm_add_region(tso_mm_region region) {

}

void __tso_mm_remove_region(tso_mm_region region) {

}

void __tso_mm_split_region(tso_mm_region previous, tso_mm_region freed_region, tso_mm_region next) {

}

void __tso_mm_join_region(tso_mm_region previous, tso_mm_region freed_region, tso_mm_region next) {

}
