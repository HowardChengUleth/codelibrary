long long fast_exp_mod(long long b, long long n, long long m) {
  if (n == 0) return 1 % m;
  if (n % 2 == 0) return fast_exp_mod((b*b)%m, n/2, m);
  return (fast_exp_mod(b, n-1, m) * b) % m;
}
