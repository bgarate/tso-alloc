#include tsoalloc.h

#DEFINE START_NEXT_FREE(dsd)
#DEFINE SIZE_NEXT_FREE(asdasd)


static ALLOCATION_STRATEGY current_strategy = FIRST_FIT;


tso_mm_alloc(size_t size) {

}

tso_mm_free(void* addr) {

}

tso_mm_switch_strategy(ALLOCATION_STRATEGY strategy) {

}

__tso_mm_initialize() {

}

__tso_mm_expand() {

}

__tso_mm_get_fit(size_t size) {
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

__tso_mm_get_best_fit(size_t size) {
}

__tso_mm_get_worse_fit(size_t size) {
}

__tso_mm_get_first_fit(size_t size) {
}

__tso_mm_add_region(tso_mm_region region) {

}

__tso_mm_remove_region(tso_mm_region region) {

}

__tso_mm_split_region(tso_mm_region previous, tso_mm_region freed_region, tso_mm_region next) {

}

__tso_mm_join_region(tso_mm_region previous, tso_mm_region freed_region, tso_mm_region next) {

}
