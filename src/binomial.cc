// Binomial Coefficients
//
// Two ways to compute binomial coefficients:
//
//   - one way computes all binomial coefficients with n <= MAX_N O(MAX_N^2)
//   - one way computes a single binomial coefficient O(k)
//
// Author: Howard Cheng and Cody Barnson
//

#include <iostream>

using namespace std;

typedef long long ll;

// computes all binomial coefficients up to MAX_N.  Read them off the table
// after calling precomp().  O(MAX_N^2)
const int MAX_N = 10;
ll binom[MAX_N+1][MAX_N+1];
void precomp()
{
  for (int n = 0; n <= MAX_N; n++) {
    binom[n][0] = binom[n][n] = 1;
    for (int k = 1; k < n; k++) {
      binom[n][k] = binom[n-1][k] + binom[n-1][k-1];
    }
  }
}

// computes single binomial coefficient C(n, k)   O(k)
ll binom(int n, int k)
{
  if (k == 0 || k == n) return 1;
  k = min(k, n - k);
  ll ans = 1;
  for (ll i = 1; i <= k; i++) {
    ans *= (n - k + i);
    ans /= i;
  }
  return ans;
}
