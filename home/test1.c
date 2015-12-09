#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{

  system("echo SIMPLE ALLOC TEST");

  // print memory map
  system("echo Mapa de memoria antes");
  char str[80];
  sprintf(str, "cat /proc/%ld/maps", (int)getpid());
  system(str);

  int* a = tso_alloc(sizeof(int));

  sprintf(str, "echo Variable a location: %lu", a);
  system(str);

  *a = 50;

  sprintf(str, "echo Variable a value: %d", *a);
  system(str);
  
  // print memory map
  system("echo Mapa de memoria despues");
  sprintf(str, "cat /proc/%ld/maps", (int)getpid());
  system(str);

  assert(*a == 50);
  return 0;

}