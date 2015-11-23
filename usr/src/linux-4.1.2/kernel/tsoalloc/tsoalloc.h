
#define INITIAL_SIZE 8

struct tso_mm_free_region {
  struct list_head list;
}

struct tso_mm_mapping {
  addr_t start;
  size_t size;
};

tso_mm_mapping* init_tsoalloc();
