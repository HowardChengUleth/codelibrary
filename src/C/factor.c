/*
 * Prime Factorization
 *
 * Author: Ethan Kim
 * Complexity: O(sqrt(n))
 *
 * Takes an integer and writes out the prime factorization in
 * ascending order. Prints -1 first, when given a negative integer.
 *
 * Note: you can change this code to store the factors in an array or process
 * the factors in other ways.
 * 
 * Also, this code works for all integers even on INT_MIN (note that negating
 * INT_MIN does nothing, but it still works because INT_MIN is a power of 2).
 *
 */

#include <stdio.h>

void factor(int n) {
  int printed = 0;
  long long p;

  if (n == 0 || n == 1) {
    printf("%d\n", n);
    return;
  }
  if (n < 0) {
    n *= -1;
    printf("-1\n");
    printed = 1;
  }

  while (n % 2 == 0) {
    n/=2;
    printf("2\n");
    printed = 1;
  }

  for (p = 3; p*p <= n; p += 2) {
    while (n % p == 0) {
      n /= p;
      printf("%lld\n", p);
      printed = 1;
    }
  }

  if(n>1 || !printed)
    printf("%d\n", n);
}

int main(void) {
  int p;
  while(scanf("%d", &p) == 1) {
    if(!p)
      break;
    factor(p);
  }
  return 0;
}










