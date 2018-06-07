/*
 * Multiplication/division without overflow
 *
 * Author: Howard Cheng
 *
 * Given a list of factors in the numerator (num, size n) and a list
 * of factors in the denominator (dem, size m), it returns the product
 * of the numerator divided by the denominator, while reducing the 
 * result as soon as it is larger than some BOUND.
 *
 */

#include <stdio.h>
#include <assert.h>

#define BOUND (1 << 16)

double mult(double *num, int n, double *dem, int m)
{
   int i, j;
   double prod = 1.0;
   i = j = 0;
   while (i < n || j < m) {
      if (prod >= BOUND && j < m) {
         prod /= dem[j++];
      } else if (i < n) {
         prod *= num[i++];
      } else {
         assert(j < m);
         prod /= dem[j++];
      }
   }
   return prod;
}

int main(void)
{
  double A[1000], B[1000];
  int n, m, i;

  while (scanf("%d %d", &n, &m) == 2 && n > 0 && m > 0) {
    for (i = 0; i < n; i++) {
      scanf("%lf", &A[i]);
    }
    for (i = 0; i < m; i++) {
      scanf("%lf", &B[i]);
    }
    printf("prod = %.4f\n", mult(A,n,B,m));
  }

  return 0;
}
