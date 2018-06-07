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

#include <iostream>

using namespace std;

void factor(int n) {
  int printed = 0;
  long long p;

  if (n == 0 || n == 1) {
    cout << n << endl;
    return;
  }
  if (n < 0) {
    n *= -1;
    cout << "-1" << endl;
    printed = 1;
  }

  while (n % 2 == 0) {
    n/=2;
    cout << "2" << endl;
    printed = 1;
  }

  for (p = 3; p*p <= n; p += 2) {
    while (n % p == 0) {
      n /= p;
      cout << p << endl;
      printed = 1;
    }
  }

  if(n>1 || !printed)
    cout << n << endl;
}

int main(void) {
  int p;
  while(cin >> p && p != 0) {
    factor(p);
  }
  return 0;
}










