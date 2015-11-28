#include <linux/tsoalloc.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/slab.h>

#include <linux/mm.h>
#include <asm/mman.h>
//#include <sys/mman.h>

#define current_mm current->tso_mm

static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;

enum FIT_CONDITIONS {
  OK,
  START,
  NO_FIT
}

inline void* __region_end(struct tso_mm_region * region) {
  return (void*)(&region + sizeof(struct tso_mm_region) + region->size);
}

struct tso_mm_mapping * tso_mm_initialize(void) {
  struct tso_mm_mapping * mm = kmalloc(sizeof(struct tso_mm_mapping), GFP_KERNEL);

  mm->start = (void*) vm_mmap(NULL, NULL, INITIAL_SIZE, PROT_READ | PROT_WRITE, 0, 0);
  mm->size = INITIAL_SIZE;
  mm->free = INITIAL_SIZE;
  mm->first_region = NULL;

  return mm;
}

void __tso_mm_finalize(void) {

}

void __tso_mm_expand(void) {
  
}

inline size_t region_size(size_t size) {
  return sizeof(struct tso_mm_region) + size;
}

struct tso_mm_region* tso_mm_get_fit(size_t size, enum FIT_CONDITIONS* state) {

  struct tso_mm_region* next_region = current_mm->first_region;

  if(next_region == NULL) {
    state = START;
    return NULL;
  }

  struct tso_mm_region* previous_region = NULL;
  struct tso_mm_region* best_match = NULL;

  long best_size = 0;
  long available_size = &next_region - current_mm->start;

  while(next_region != NULL) {

    if(available_size > region_size(size)) {

      bool isBest = false;

      switch(current_strategy) {
        case WORST_FIT:
          isBest = available_size < best_size;
          break;
        case BEST_FIT:
          isBest = available_size > best_size;
          break;
        case FIRST_FIT:
          state = OK;
          return (tso_mm_region*)&previous_region;
        }

        if (isBest) {
          best_size = available_size;
          best_match = previous_region;
        }

    }

    previous_region = next_region;
    next_region = next_region->next;
    available_size = &next_region - &previous_region - sizeof(struct tso_mm_region);
  }

  state = best_match != NULL ? OK : NO_FIT;
  return best_match;

}

asmlinkage long sys_tso_mm_alloc(size_t size, void* address) {
  struct tso_mm_region* new_region;

  address = NULL;

  if (current_mm == NULL)
    tso_mm_initialize();

  if(current_mm->free < size)
    return -1;

  enum FIT_CONDITIONS state;
  struct tso_mm_region* fit = tso_mm_get_before_fit(size, &size);

  long position;

  switch(state) {
    case OK:
      position = &fit + sizeof(tso_mm_region);
    case START:
      position = current_mm->start;
      break;
    default:
      return -1;
  }
  
  new_region = (struct tso_mm_region*)(position);
  new_region->size = size;

  new_region->next = fit->next;
  
  if(state == OK)
    fit->next = new_region;
  else if(state == START)
    current_mm->first_region = fit;


  void* res = (void*)(&new_region + sizeof(tso_mm_region))
  copy_to_user(&address, res, sizeof(void*));

  return 0;
}

asmlinkage long sys_tso_mm_free(void* addr) {
  return 0;
}

asmlinkage long sys_tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {
  current_strategy = strategy;
  return 0;
}

