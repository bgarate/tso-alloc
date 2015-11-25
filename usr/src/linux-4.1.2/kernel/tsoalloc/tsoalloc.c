#include <linux/tsoalloc.h>
#include <linux/list.h>

#define current_mm current->tso_mm

static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;

asmlinkage long sys_tso_mm_alloc(size_t size, void* ret) {
  
  if(current_mm->free < size)
    return NULL;

  tso_mm_region* fit = __tso_mm_get_before_fit(size);

  if(fit == NULL)
    return NULL;

  tso_mm_region* new_region = (tso_mm_region*)(__region_end(fit) + 1);
  new_region->size = size;

<<<<<<< HEAD
  new_region->next = fit->next;
  fit->next = new_region;

  ret = __to_user_address(new_region);

=======
asmlinkage long sys_tso_mm_alloc(size_t size) {
  return 0;
>>>>>>> origin/master
}

asmlinkage long sys_tso_mm_free(void* addr) {
  return 0;
}

<<<<<<< HEAD
asmlinkage long sys_tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {
  current_strategy = strategy;
}

inline void* __region_end(tso_mm_region region) {
  return (void*)(&region + sizeof(tso_mm_region) + region->size)
=======
asmlinkage long sys_tso_mm_switch_strategy(enum ALLOCATION_STRATEGY strategy) {
  return 0;
>>>>>>> origin/master
}

tso_mm_maping* __tso_mm_initialize() {
  tso_mm_maping* mm = kalloc(sizeof(tso_mm_maping))

  mm->start = do_mmap(NULL, NULL, INITIAL_SIZE, PROT_READ | PROT_WRITE, 0, 0);
  mm->size = INTIAL_SIZE;
  mm->free = INITIAL_SIZE;
  mm->first_region = NULL;

  return tso_mm_maping;
}

void __tso_mm_finalize() {

}

inline struct tso_mm_region* __tso_first_region() {
  return (tso_mm_region*)current_mm->start
}

inline struct tso_mm_region* __tso_first_region() {
  return (tso_mm_region*)current_mm->start
}

void __tso_mm_expand() {
  
}

tso_mm_region* __tso_mm_get_before_fit(size_t size) {
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

<<<<<<< HEAD
tso_mm_region* __tso_mm_get_before_best_fit(size_t size) {
}

tso_mm_region* __tso_mm_get_before_worse_fit(size_t size) {
}

tso_mm_region* __tso_mm_get_before_first_fit(size_t size) {
  tso_mm_region current_region = __tso_first_region();

  if(current_region == NULL)
    return 
=======
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
>>>>>>> origin/master

}

void __tso_mm_remove_region(struct tso_mm_region region) {

}

void __tso_mm_split_region(struct tso_mm_region previous, struct tso_mm_region freed_region, struct tso_mm_region next) {

}

void __tso_mm_join_region(struct tso_mm_region previous, struct tso_mm_region freed_region, struct tso_mm_region next) {

}
