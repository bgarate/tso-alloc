#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

  printf("BEST FIT TEST\n");

  tso_switch_strategy(BEST_FIT);

  void* a = tso_alloc(40);
  void* b = tso_alloc(60);
  void* c = tso_alloc(80);
  void* d = tso_alloc(100);
  void* e = tso_alloc(130);
  void* f = tso_alloc(200);
  void* g = tso_alloc(280);

  printf("Variable a location: %p\n", a);
  printf("Variable b location: %p\n", b);
  printf("Variable c location: %p\n", c);
  printf("Variable d location: %p\n", d);
  printf("Variable e location: %p\n", e);
  printf("Variable f location: %p\n", f);
  printf("Variable g location: %p\n", g);

  tso_free(c);
  tso_free(e);
  tso_free(f);

  void* h = tso_alloc(20);
  void* i = tso_alloc(60);
  void* j = tso_alloc(40);

  printf("Variable h location: %p\n", h);
  printf("Variable i location: %p\n", i);
  printf("Variable j location: %p\n", j);

  return 0;

}
