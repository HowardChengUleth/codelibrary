// Compute nth Fibonacci number with matrix exponentiation
//
// Time complexity: O(log(n))
//
// Author: Cody Barnson
//
// Warning: 46th Fibonacci number (i.e. fib(46)) is largest
// that will fit into signed 32-bit integer; use long long if need
// longer.  Or perhaps the problem asks for Fibonacci number mod m

int f[1000];
int fib(int n) {
  if (n < 2) return n;
  if (f[n]) return f[n];

  int k = (n + 1) / 2;
  f[n] = (n & 1) ? fib(k) * fib(k) + fib(k - 1) * fib(k - 1)
                 : (2 * fib(k - 1) + fib(k)) * fib(k);
  return f[n];
}
