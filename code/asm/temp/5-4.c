#include <stdio.h>

int main (int argc, char *argv[])
{
  int temp = 0x87654321;
  printf("%x\n", temp);
  int low = temp & 0xffff;
  printf("%x\n", low);
  int high = (temp >> 16) & 0xffff;
  printf("%x\n", high);
  return 0;
}
