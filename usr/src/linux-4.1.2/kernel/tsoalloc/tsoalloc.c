#include <linux/tsoalloc.h>
#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <asm/mman.h>
#include <linux/types.h>

#define current_mm current->tso_mm

static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;

enum FIT_CONDITIONS {
  OK,
  START,
  NO_FIT
};

inline void* __region_end(struct tso_mm_region * region) {
  return (void*)(region + (unsigned long)sizeof(struct tso_mm_region) + region->size);
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

  printk("Start: %p | Size: %lu | Free: %lu | First region: %p\n", mm->start,
    mm->size, mm->free, mm->first_region);

  printk("Initialization alloc finished\n");
  return mm;
}

void __tso_mm_finalize(void) {
  printk("ERROR: Finalize not implemented\n");
}

void __tso_mm_expand(void) {
  printk("ERROR: Expand not implemented\n");
}

inline unsigned long region_size(unsigned long size) {
  return ((unsigned long)sizeof(struct tso_mm_region)) + size;
}

struct tso_mm_region* tso_mm_get_fit(unsigned long size, enum FIT_CONDITIONS* state) {

  struct tso_mm_region* next_region = current_mm->first_region;
  struct tso_mm_region* previous_region = NULL;
  struct tso_mm_region* best_match = NULL;
  unsigned long best_size = 0;
  unsigned long available_size;
  int isBest;

  if(next_region == NULL) {
    printk("Case start at begin\n");
    *state = START;
    return NULL;
  }

  available_size = ((unsigned long)next_region - ((unsigned long)current_mm->start));
  switch(current_strategy) {
    case WORST_FIT:
      best_size = 0;
      break;
    case BEST_FIT:
      best_size = current_mm->size+1;
      break;
    case FIRST_FIT:
      break;
  }

  //int i = 0;
  while(1/*i<5*/) {
    //i++;

    printk("Available Size: %lu \n", available_size);
    if(available_size > region_size(size)) {

      printk("Size enough!\n");
      *state = OK;
      isBest = 0;

      printk("Current strategy %u\n", current_strategy);
      switch(current_strategy) {
        case WORST_FIT:
          printk("Case WORST_FIT\n");
          isBest = available_size > best_size;
          break;
        case BEST_FIT:
          printk("Case BEST_FIT!\n");
          isBest = available_size < best_size;
          break;
        case FIRST_FIT:
          printk("Case FIRST_FIT!\n");
          if (previous_region == NULL){
            *state=START;
          }
          return (struct tso_mm_region*)previous_region;
      }

        if (isBest) {
          printk("Case is best!\n");
          best_size = available_size;
          best_match = previous_region;
        }

    }

    previous_region = next_region;
    if (previous_region == NULL){
      break;
    }
    next_region = next_region->next;
    if (next_region != NULL)
      available_size = (unsigned long)next_region - (unsigned long)previous_region - region_size(previous_region->size);
    else 
      available_size = (unsigned long)current_mm->start + current_mm->size - (unsigned long)previous_region - region_size(previous_region->size);
  }

  printk("Best Match: %p \n", best_match);
  if ((*state == OK) && (best_match == NULL)){
    *state=START;
  }
  return best_match;

}

asmlinkage long sys_tso_mm_alloc(size_t size, void** address) {
  struct tso_mm_region* new_region;
  enum FIT_CONDITIONS state = NO_FIT;
  struct tso_mm_region* fit;
  unsigned long position;
  void* res;

  //*address = NULL;

  if (current_mm == NULL)
    current_mm = tso_mm_initialize();

  printk("Current_mm after initialize:  %p\n", current_mm);

  if(current_mm->free < region_size(size))
    return -1;//Tendia que agrandar el vma

  printk("State value before get fit %u\n", state);

  fit = tso_mm_get_fit(size, &state);

  printk("State value after get fit: %u\n", state);
  printk("fit value: %p\n", fit);

  switch(state) {
    case OK:
      position = ((unsigned long)fit) + region_size(fit->size);
      break;
    case START:
      position = (unsigned long)current_mm->start;
      break;
    default:
      return -1;
  }

  printk("position value: %lu\n", position);
  printk("Position setted, creating new region\n");  

  new_region = (struct tso_mm_region*)(position);
  new_region->size = (unsigned long)size;

  if (state == START){
    new_region->next = current_mm->first_region;
  }else{
    new_region->next = fit->next;  
  }

  printk("New region created, updating mm\n");  

  if(state == OK)
    fit->next = new_region;
  else if(state == START)
    current_mm->first_region = new_region;

  current_mm->free -= (unsigned long)size + (unsigned long)sizeof(struct tso_mm_region);

  printk("mm updated, setting return value\n");

  res = (void*)((unsigned long)new_region + (unsigned long)sizeof(struct tso_mm_region));
  printk("Final result: %p\n", res);
  copy_to_user(address, &res, (unsigned long)sizeof(void*));

  printk("Start: %p | Size: %lu | Free: %lu | First region: %p\n", current_mm->start,
    current_mm->size, current_mm->free, current_mm->first_region);
  printk("Alloc address: %p\n\n", res);

  return 0;
}

asmlinkage long sys_tso_mm_free(void* addr) {
  struct tso_mm_region* region;
  struct tso_mm_region* previous_region;
  void* dataAdd;

  printk("(before free) Start: %p | Size: %lu | Free: %lu | First region: %p\n", current_mm->start,
    current_mm->size, current_mm->free, current_mm->first_region);
  printk("addr: %p \n", addr);

  if (current_mm->first_region == NULL){
    return -1;

  } else {

    printk("sizeof : %u \n", sizeof(struct tso_mm_region));
    printk("sizeof long: %lu \n", (unsigned long)sizeof(struct tso_mm_region));

    region = current_mm->first_region;
    dataAdd = (void*)((unsigned long)region + (unsigned long)sizeof(struct tso_mm_region));

    printk("dataAdd: %p \n", dataAdd);
    if (((unsigned long)dataAdd) == ((unsigned long)addr)){
      current_mm->first_region = region->next;
      current_mm->free += region_size(region->size);
      printk("free fisrt region match!\n");
    } else {

      previous_region = region;
      region = region->next;
      dataAdd = (void*)((unsigned long)region + (unsigned long)sizeof(struct tso_mm_region));

      while(region != NULL){
        printk("dataAdd: %lu \n", (unsigned long) dataAdd);
        if (((unsigned long)dataAdd) == ((unsigned long)addr)){
          printk("free matched!\n");
          previous_region->next = region->next;
          current_mm->free += region_size(region->size);
          break;
        } else {

          previous_region = region;
          region = region->next;
          if (region == NULL)
            return -1;
          dataAdd = (void*)((unsigned long)region + (unsigned long)sizeof(struct tso_mm_region)); 
        }
      }  
    }
  }

  printk("(after free) Start: %p | Size: %lu | Free: %lu | First region: %p\n", current_mm->start,
    current_mm->size, current_mm->free, current_mm->first_region);

  return 0;
}

asmlinkage long sys_tso_mm_switch_strategy(unsigned int strategy) {
  printk("strategy recived %u", current_strategy);
  //copy_from_user(&current_strategy, &strategy, sizeof(ALLOCATION_STRATEGY));
  current_strategy = (ALLOCATION_STRATEGY)strategy;
  return 0;
}

