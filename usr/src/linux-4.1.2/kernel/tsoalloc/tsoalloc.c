#include <linux/tsoalloc.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <asm/mman.h>

#define current_mm current->tso_mm

static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;

enum FIT_CONDITIONS {
  OK,
  START,
  NO_FIT
};

inline void* __region_end(struct tso_mm_region * region) {
  return (void*)(long)(region + sizeof(struct tso_mm_region) + region->size);
}

struct tso_mm_mapping * tso_mm_initialize(void) {

  struct tso_mm_mapping * mm;
  unsigned long addr;

  printk("\n\nInitializing alloc\n");

  mm = kmalloc(sizeof(struct tso_mm_mapping), GFP_KERNEL);

  addr = vm_mmap(NULL, 0, INITIAL_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0);
  printk("Dirr vm_mmap: %lu \n", addr);
  
  mm->start = (void*) addr; 
  mm->size = INITIAL_SIZE;
  mm->free = INITIAL_SIZE;
  mm->first_region = NULL;

  printk("Start: %p | Size: %d | Free: %d | First region: %p\n", mm->start,
    mm->size, mm->free, mm->first_region);

  return mm;
}

void __tso_mm_finalize(void) {
  printk("ERROR: Finalize not implemented\n");
}

void __tso_mm_expand(void) {
  printk("ERROR: Expand not implemented\n");
}

inline size_t region_size(size_t size) {
  return sizeof(struct tso_mm_region) + size;
}

struct tso_mm_region* tso_mm_get_fit(size_t size, enum FIT_CONDITIONS* state) {

  struct tso_mm_region* next_region = current_mm->first_region;
  struct tso_mm_region* previous_region = NULL;
  struct tso_mm_region* best_match = NULL;
  long best_size = 0;
  long available_size;

  if(next_region == NULL) {
    *state = START;
    return NULL;
  }

  available_size = (long)(next_region - ((long)current_mm->start));

  while(next_region != NULL) {

    if(available_size > region_size(size)) {

      bool isBest = false;

      switch(current_strategy) {
        case WORST_FIT:
          isBest = available_size > best_size;
          break;
        case BEST_FIT:
          isBest = available_size < best_size;
          break;
        case FIRST_FIT:
          state = OK;
          return (struct tso_mm_region*)previous_region;
      }

        if (isBest) {
          best_size = available_size;
          best_match = previous_region;
        }

    }

    previous_region = next_region;
    next_region = next_region->next;
    available_size = next_region - previous_region - sizeof(struct tso_mm_region);
  }

  *state = best_match != NULL ? OK : NO_FIT;
  return best_match;

}

asmlinkage long sys_tso_mm_alloc(size_t size, void** address) {
  struct tso_mm_region* new_region;
  enum FIT_CONDITIONS state;
  struct tso_mm_region* fit;
  long position;
  void* res;

  address = NULL;

  if (current_mm == NULL)
    tso_mm_initialize();

  if(current_mm->free < size)
    return -1;//Tendia que agrandar el vma

  //fit = tso_mm_get_before_fit(size, &state);
  fit = tso_mm_get_fit(size, &state);

  switch(state) {
    case OK:
      //position = ((long)fit) + sizeof(struct tso_mm_region);//mal fit apunta a la region anterior
      position = ((long)fit) + ((long)fit->size) + sizeof(struct tso_mm_region);
    case START:
      position = (long)current_mm->start;
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

  //current_mm->size -= available_size;//no exixte available_size en esta funcion y el size no varia
  current_mm->free -= size + sizeof(struct tso_mm_region);

  res = (void*)(long)(new_region + sizeof(struct tso_mm_region));
  copy_to_user(*address, res, sizeof(void*));

  printk("Start: %ld | Size: %d | Free: %d | First region: %p\n", (long)current_mm->start,
    current_mm->size, current_mm->free, current_mm->first_region);
  printk("Fit address: %p\n", fit);
  printk("Alloc address: %p\n", res);

  return 0;
}

asmlinkage long sys_tso_mm_free(void* addr) {
  return 0;
}

asmlinkage long sys_tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {
  current_strategy = strategy;
  return 0;
}

