#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
  char * aux = "";

  system("echo SIMPLE ALLOC TEST");

  // print memory map
  system("echo Mapa de memoria");
  char str[80];
  sprintf(str, "cat /proc/%ld/maps", (int)getpid());
  system(str);

  int* a = tso_alloc(sizeof(int));

  sprintf(aux, "echo Variable a location: %p", a);
  system(aux);

  *a = 50;

  sprintf(aux, "echo Variable a value: %d", *a);
  system(aux);
  assert(*a == 50);

  return 0;

}