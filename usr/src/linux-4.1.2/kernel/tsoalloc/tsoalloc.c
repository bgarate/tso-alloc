#include <linux/tsoalloc.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/slab.h>

#define current_mm current->tso_mm

static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;

inline void* __region_end(struct tso_mm_region * region) {
  return (void*)(&region + sizeof(struct tso_mm_region) + region->size);
}

struct tso_mm_mapping * __tso_mm_initialize(void) {
  struct tso_mm_mapping * mm = kmalloc(sizeof(struct tso_mm_mapping), GFP_KERNEL);

  mm->start = (void*) do_mmap(NULL, NULL, INITIAL_SIZE, PROT_READ | PROT_WRITE, 0, 0);
  mm->size = INTIAL_SIZE;
  mm->free = INITIAL_SIZE;
  mm->first_region = NULL;

  return mm;
}

void __tso_mm_finalize(void) {

}

inline struct tso_mm_region* __tso_first_region(void) {
  return (struct tso_mm_region*)current_mm->start;
}

void __tso_mm_expand(void) {
  
}

struct tso_mm_region* __tso_mm_get_before_best_fit(size_t size) {
}

struct tso_mm_region* __tso_mm_get_before_worst_fit(size_t size) {
}

struct tso_mm_region* __tso_mm_get_before_first_fit(size_t size) {
  struct tso_mm_region * current_region = __tso_first_region();

  //if(current_region == NULL)
  //  return NULL;

  return NULL;
}

struct tso_mm_region* __tso_mm_get_before_fit(size_t size) {
  switch(current_strategy) {
  case WORST_FIT:
    return __tso_mm_get_before_worst_fit(size);
    break;
  case BEST_FIT:
    return __tso_mm_get_before_best_fit(size);
    break;
  case FIRST_FIT:
    return __tso_mm_get_before_first_fit(size);
    break;
  }
}

void __tso_mm_remove_region(struct tso_mm_region * region) {

}

void __tso_mm_split_region(struct tso_mm_region * previous, struct tso_mm_region * freed_region, struct tso_mm_region * next) {

}

void __tso_mm_join_region(struct tso_mm_region * previous, struct tso_mm_region * freed_region, struct tso_mm_region * next) {

}

asmlinkage long sys_tso_mm_alloc(size_t size, void* ret) {
  struct tso_mm_region* new_region;

  if(current_mm->free < size)
    return -1;

  struct tso_mm_region* fit = __tso_mm_get_before_fit(size);

  if(fit == NULL)
    return -1;

  new_region = (struct tso_mm_region*)(__region_end(fit) + 1);
  new_region->size = size;

  new_region->next = fit->next;
  fit->next = new_region;

  //ret = __to_user_address(new_region);

  return 0;
}

asmlinkage long sys_tso_mm_free(void* addr) {
  return 0;
}

asmlinkage long sys_tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {
  current_strategy = strategy;
  return 0;
}

