/*
 * Chinese Remainder Theorem
 *
 * Author: Howard Cheng
 * Reference:
 *   Geddes, K.O., Czapor, S.R., and Labahn, G.  Algorithms for Computer 
 *   Algebra, Kluwer Academic Publishers, 1992, p. 180
 *
 * Given n relatively prime modular in m[0], ..., m[n-1], and right-hand
 * sides a[0], ..., a[n-1], the routine solves for the unique solution
 * in the range 0 <= x < m[0]*m[1]*...*m[n-1] such that x = a[i] mod m[i]
 * for all 0 <= i < n.  The algorithm used is Garner's algorithm, which
 * is not the same as the one usually used in number theory textbooks.
 *
 * It is assumed that m[i] are positive and pairwise relatively prime.
 * a[i] can be any integer.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

int gcd(int a, int b, int &s, int &t)
{
  int r, r1, r2, a1, a2, b1, b2, q;
  int A = a;
  int B = b;

  a1 = b2 = 1;
  a2 = b1 = 0;

  while (b) {
    assert(a1*A + a2*B == a);
    q = a / b;
    r = a % b;
    r1 = a1 - q*b1;
    r2 = a2 - q*b2;
    a = b;
    a1 = b1;
    a2 = b2;
    b = r;
    b1 = r1;
    b2 = r2;
  }

  s = a1;
  t = a2;
  assert(a >= 0);
  return a;
}

int cra(int n, int m[], int a[])
{
  int x, i, k, prod, temp;
  int *gamma, *v;

  gamma = new int[n];
  v = new int[n];
  assert(gamma && v);

  /* compute inverses */
  for (k = 1; k < n; k++) {
    prod = m[0] % m[k];
    for (i = 1; i < k; i++) {
      prod = (prod * m[i]) % m[k];
    }
    gcd(prod, m[k], gamma[k], temp);
    gamma[k] %= m[k];
    if (gamma[k] < 0) {
      gamma[k] += m[k];
    }
  }

  /* compute coefficients */
  v[0] = a[0];
  for (k = 1; k < n; k++) {
    temp = v[k-1];
    for (i = k-2; i >= 0; i--) {
      temp = (temp * m[i] + v[i]) % m[k];
      if (temp < 0) {
	temp += m[k];
      }
    }
    v[k] = ((a[k] - temp) * gamma[k]) % m[k];
    if (v[k] < 0) {
      v[k] += m[k];
    }
  }

  /* convert from mixed-radix representation */
  x = v[n-1];
  for (k = n-2; k >= 0; k--) {
    x = x * m[k] + v[k];
  } 

  delete[] gamma;
  delete[] v;

  return x;
}


int main(void)
{
  int n, *m, *a, i, x;

  while (cin >> n && n > 0) {
    m = new int[n];
    a = new int[n];
    assert(m && a);
    cout << "Enter moduli:" << endl;
    for (i = 0; i < n; i++) {
      cin >> m[i];
    }
    cout << "Enter right-hand side:" << endl;
    for (i = 0; i < n; i++) {
      cin >> a[i];
    }
    x = cra(n, m, a);
    cout << "x = " << x << endl;

    for (i = 0; i < n; i++) {
      assert((x-a[i]) % m[i] == 0);
    }

    delete[] m;
    delete[] a;
  }
  return 0;
}

