#include "tso_lib/tso_lib.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  system("echo SIMPLE ALLOC TEST");
  char str[500];

  int* a = tso_alloc(sizeof(int));
  int* b = tso_alloc(sizeof(int));
  long* c = tso_alloc(sizeof(long));

  sprintf(str, "echo 'a %p | b %p | c %p'", a, b, c);
  system(str);

  *a = 50;
  *b = 100;
  *c = 500;

  sprintf(str, "echo Variable a value: %d", *a);
  system(str);
  sprintf(str, "echo Variable b value: %d", *b);
  system(str);
  sprintf(str, "echo Variable c value: %d", *c);
  system(str);

  return 0;

}
