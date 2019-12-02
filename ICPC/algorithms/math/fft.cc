#define double long double
const double pi = acos(-1);
typedef complex<double> cd;
typedef vector<cd> vcd;
void fft(vcd& A, int n, bool inv) {
  if (n == 1) return;
  vcd e(n/2), o(n/2);
  for (int i = 0; i < n; i++) (i & 1 ? o : e)[i/2] = A[i];
  fft(e, n/2, inv); fft(o, n/2, inv);
  double a = (inv ? -1 : 1) * 2.0 * pi / n;
  cd w(1), wn(cos(a), sin(a));
  for (int i = 0; i < n/2; i++) {
    A[i] = e[i] + w * o[i];
    A[i + n/2] = e[i] - w * o[i];
    if (inv) A[i] /= 2, A[i + n/2] /= 2;
    w *= wn;
  }
}

vcd mult(vcd A, vcd B) {
  int n = sz(A), m = sz(B), N = 1;
  while (N < n + m) N <<= 1;
  A.resize(N); B.resize(N);
  fft(A, N, 0); fft(B, N, 0); vcd ans(N);
  for (int i = 0; i < N; i++) ans[i] = A[i] * B[i];
  fft(ans, N, 1);
  return ans;
}
