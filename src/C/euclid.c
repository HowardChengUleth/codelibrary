/*
 * Euclidean Algorithm
 *
 * Author: Howard Cheng
 *
 * Given two integers, return their gcd.
 *
 */

#include <stdio.h>
#include <assert.h>


int gcd(int a, int b)
{
  int r;

  /* unnecessary if a, b >= 0 */
  if (a < 0) {
    a = -a;
  }
  if (b < 0) {
    b = -b;
  }

  while (b) {
    r = a % b;
    a = b;
    b = r;
  }
  assert(a >= 0);
  return a;
}

int main(void)
{
  int a, b;

  while (scanf("%d %d", &a, &b) == 2) {
    printf("%d\n", gcd(a, b));
  }
  return 0;
}
