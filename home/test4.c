#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

  printf("BEST FIT TEST");

  tso_switch_strategy(BEST_FIT);

  void* a = tso_alloc(40);
  void* b = tso_alloc(60);
  void* c = tso_alloc(80);
  void* d = tso_alloc(100);
  void* e = tso_alloc(130);
  void* f = tso_alloc(200);
  void* g = tso_alloc(280);

  printf("Variable a location: %p", a);
  printf("Variable b location: %p", b);
  printf("Variable c location: %p", c);
  printf("Variable d location: %p", d);
  printf("Variable e location: %p", e);
  printf("Variable f location: %p", f);
  printf("Variable g location: %p", g);

  tso_free(c);
  tso_free(e);
  tso_free(f);

  void* h = tso_alloc(20);
  void* i = tso_alloc(60);
  void* j = tso_alloc(40);

  printf("Variable h location: %p", h);
  printf("Variable i location: %p", i);
  printf("Variable j location: %p", j);

  return 0;

}
