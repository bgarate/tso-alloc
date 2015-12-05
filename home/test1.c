#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
  char * aux = "";

  perror("SIMPLE ALLOC TEST");

  int* a = tso_alloc(sizeof(int));

  sprintf(aux, "Variable a location: %p", a);
  perror(aux);

  *a = 50;

  sprintf(aux, "Variable a value: %d", *a);
  perror(aux);
  assert(*a == 50);

  return 0;

}