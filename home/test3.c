#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

  printf("FIRST FIT TEST");

  void* a = tso_alloc(40);
  void* b = tso_alloc(60);
  void* c = tso_alloc(80);
  void* d = tso_alloc(100);

  printf("Variable a location: %p", a);
  printf("Variable b location: %p", b);
  printf("Variable c location: %p", c);
  printf("Variable d location: %p", d);

  tso_free(c);

  void* e = tso_alloc(20);
  void* f = tso_alloc(80);
  void* g = tso_alloc(20);

  printf("Variable e location: %p", e);
  printf("Variable f location: %p", f);
  printf("Variable g location: %p", g);

  tso_free(a);
  tso_free(b);

  void* h = tso_alloc(50);

  printf("Variable h location: %p", h);

  return 0;

}
