#include <stdio.h>

int foo (int a, int b)
{
  int c;
  asm volatile (
      "mul %1, %1, %1;"
      "mul %2, %2, %2;"
      "add %0, %1, %2;"
      :"=r"(c)
      :"r"(a), "r"(b)
      );
  return c;
}

int main ()
{
  printf("%d\n", foo(1, 2));
  return 0;
}
