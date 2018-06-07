/*
 * Integer multiplication/division without overflow
 *
 * Author: Howard Cheng
 *
 * Given a list of factors in the numerator (num, size n) and a list
 * of factors in the denominator (dem, size m), it returns the product
 * of the numerator divided by the denominator.  It is assumed that
 * the numerator is divisible by the denominator (ie. the result
 * is an integer).  Overflow will not occur as long as the final result
 * is representable.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

int gcd(int a, int b)
{
   int r;

   while (b) {
      r = a % b;
      a = b;
      b = r;
   }
   assert(a >= 0);
   return a;
}

int mult(int A[], int n, int B[], int m)
{
   int i, j, prod, d;
   int count = 0;

   /* unnecessary if the two lists are positive */
   for (i = 0; i < n; i++) {
     if (A[i] < 0) {
       A[i] *= -1;
       count++;
     }
   }
   for (i = 0; i < m; i++) {
     if (B[i] < 0) {
       B[i] *= -1;
       count++;
     }
   }

   for (i = 0; i < n; i++) {
      for (j = 0; j < m; j++) {
         d = gcd(A[i], B[j]);
         A[i] /= d;
         B[j] /= d;
      }
   }
   prod = 1;
   for (i = 0; i < n; i++) {
      prod *= A[i];
   }
   for (j = 0; j < m; j++) {
      assert(B[j] == 1);
   }
   return (count % 2 == 0) ? prod : -prod;
}

int main(void)
{
  int A[1000], B[1000], n, m, i;

  while (cin >> n >> m && n > 0 && m > 0) {
    for (i = 0; i < n; i++) {
      cin >> A[i];
    }
    for (i = 0; i < m; i++) {
      cin >> B[i];
    }
    cout << "prod = " << mult(A,n,B,m) << endl;
  }

  return 0;
}
