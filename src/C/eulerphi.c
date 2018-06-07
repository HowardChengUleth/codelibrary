/* 
 * Euler's Phi function:
 *
 * Author: Ethan Kim
 * Complexity: O(sqrt(n)) 
 *
 * Computes Euler's Phi(Totient) function; Given a positive n, computes 
 * the number of positive integers that are <= n and relatively prime to n.
 *
 * For prime n, it is easy to see that phi(n)=n-1.
 * For powers of prime, phi(p^k)=p^(k-1) * (p-1).
 * Also, phi is multiplicative, so phi(pq)=phi(p)*phi(q), if p and q are
 * relatively prime.
 *
 */

#include <stdio.h>
#include <assert.h>

int fast_exp(int b, int n)
{
  int res = 1;
  int x = b;

  while (n > 0) {
    if (n & 0x01) {
      n--;
      res *= x;
    } else {
      n >>= 1;
      x *= x;
    }
  }

  return res;
}

int phi(int n) {
  int k, res;
  long long p;

  assert(n > 0);

  res=1;
  for(k = 0; n % 2 == 0; k++) {
    n /= 2;
  }
  if (k)
    res *= fast_exp(2, k-1);

  for (p = 3; p*p <= n; p += 2) {
    for (k = 0; n % p == 0; k++) {
      n /= p;
    }
    if (k) {
      res *= fast_exp(p, k-1) * (p-1);
    }
  }
  if (n > 1) {
    res *= n-1;
  }
  return res;
}

int main(void) {
  int p;
  while(scanf("%d", &p) == 1) {
    if(!p)
      break;
    printf("%d\n", phi(p));
  }
  return 0;
}










