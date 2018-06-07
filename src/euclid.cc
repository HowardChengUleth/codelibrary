/*
 * Euclidean Algorithm
 *
 * Author: Howard Cheng
 *
 * Given two integers, return their gcd.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

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

  while (cin >> a >> b) {
    cout << gcd(a, b) << endl;
  }
  return 0;
}
