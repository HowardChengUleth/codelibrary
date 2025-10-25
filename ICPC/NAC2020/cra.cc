// solve x = a[i] mod m[i] where gcd(m[i],m[j]) | a[i]-a[j]
// x0 in [0, lcm(m's)], x = x0 + t*lcm(m's) for all t.
int cra(int n, vector<int>& m, vector<int>& a) {
  int u = a[0], v = m[0], p, q, r, t;
  for (int i = 1; i < n; i++) {
    r = gcd(v, m[i], p, q); t = v;
    if ((a[i] - u) % r != 0) { } // no solution!
    v = v/r * m[i]; u = ((a[i]-u)/r * p * t + u) % v;
  }
  if (u < 0) u += v;
  return u;
}
