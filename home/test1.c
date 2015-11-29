#include "tso_lib.h"
#include "assert.h"

int main(int argc, char const *argv[])
{

  printf("SIMPLE ALLOC TEST");

  int* a = tso_alloc(sizeof(int));

  printf("Variable a location: %p", a);

  a = 50;

  printf("Variable a value: %d", a);
  assert(*a == 50);

  return 0;

}
