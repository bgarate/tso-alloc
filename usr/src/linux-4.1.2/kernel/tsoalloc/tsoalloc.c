#include <linux/tsoalloc.h>
#include <linux/list.h>

#define current_mm current->tso_mm

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

tso_mm_maping __tso_mm_initialize() {
  tso_mm_maping mm;
  mm->start = do_mmap(NULL, NULL, INITIAL_SIZE, PROT_READ | PROT_WRITE, 0, 0);
  mm->size = INTIAL_SIZE;
  mm->free = INITIAL_SIZE;
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
