/*
 * Fast Exponentiation mod m
 *
 * Author: Howard Cheng
 *
 * Given b, n, and m, computes b^n mod m quickly.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

int fast_exp(int b, int n, int m)
{
  int res = 1;
  long long x = b;

  while (n > 0) {
    if (n & 0x01) {
      n--;
      res = (res * x) % m;
    } else {
      n >>= 1;
      x = (x * x) % m;
    }
  }

  return res;
}

int main(void)
{
  int b, n, m;

  while (cin >> b >> n >> m) {
    cout << b << "^" << n << " mod " << m << " = " << fast_exp(b, n, m)
	 << endl;
  }
  return 0;

}
