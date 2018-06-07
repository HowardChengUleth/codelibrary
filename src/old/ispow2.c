/*
 * Determines if the input is a power of 2.
 *
 * Author: Howard Cheng
 *
 * This routine determines if the input is a power of 2.
 *
 */

#include <stdio.h>

int ispow2(int x)
{
  return !((~(~0U>>1)|x)&x -1);
}

int main(void)
{
  int i;
  
  for (i = 0; i < 256; ++i) {
    printf("%3d: %d\n", i, ispow2(i));
  }
  for (i = 0; i < 30; i++) {
    printf("%3d: %d\n", i, ispow2(1 << i));
  }
  return 0;
}

