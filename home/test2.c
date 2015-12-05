#include "tso_lib.h"
#include "assert.h"

int main(int argc, char const *argv[])
{

  printf("SIMPLE FREE TEST");

  int* a = tso_alloc(sizeof(int));

  printf("Variable a location: %p", a);

  long pos_a = &a;

  tso_free(a);
  a = tso_alloc(sizeof(int));


  printf("Variable a location: %p", a);
  assert(a == pos_a);

  return 0;

}
