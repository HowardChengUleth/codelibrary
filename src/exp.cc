/*
 * Fast Exponentiation
 *
 * Author: Howard Cheng
 *
 * Given b and n, computes b^n quickly.
 *
 */

#include <iostream>

using namespace std;

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

int main(void)
{
  int b, n;

  while (cin >> b >> n) {
    cout << b << "^" << n << " = " << fast_exp(b, n) << endl;
  }
  return 0;

}
