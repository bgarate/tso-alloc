#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

  printf("FIRST FIT TEST\n");

  void* a = tso_alloc(40);
  void* b = tso_alloc(60);
  void* c = tso_alloc(80);
  void* d = tso_alloc(100);

  printf("Variable a location: %p\n", a);
  printf("Variable b location: %p\n", b);
  printf("Variable c location: %p\n", c);
  printf("Variable d location: %p\n", d);

  tso_free(c);

  void* e = tso_alloc(20);
  void* f = tso_alloc(80);
  void* g = tso_alloc(20);

  printf("Variable e location: %p\n", e);
  printf("Variable f location: %p\n", f);
  printf("Variable g location: %p\n", g);

  tso_free(a);
  tso_free(b);

  void* h = tso_alloc(50);

  printf("Variable h location: %p\n", h);

  return 0;

}
