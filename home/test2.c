#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{

  printf("SIMPLE FREE TEST\n");

  int* a = tso_alloc(sizeof(int));

  printf("Variable a location: %p\n", a);

  unsigned long pos_a = (unsigned long)a;

  tso_free(a);
  a = tso_alloc(sizeof(int));


  printf("Variable a location: %p\n", a);
  assert(((unsigned long)a) == pos_a);

  return 0;

}
