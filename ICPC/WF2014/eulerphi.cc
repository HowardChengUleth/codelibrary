// Euler Phi
int exp(int b, int n) {
  return (n == 0) ? 1 : b * exp(b, n-1);
}

int phi(int n) {
  int k, res = 1;
  for (k = 0; n % 2 == 0; k++)      n /= 2;
  if (k) res *= exp(2, k-1);
  for (long long p = 3; p*p <= n; p += 2) {
    for (k = 0; n % p == 0; k++)    n /= p;
    if (k) res *= exp(p, k-1) * (p-1);
  }
  if (n > 1) res *= n-1;
  return res;
}








