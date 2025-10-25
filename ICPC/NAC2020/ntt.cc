/*  number theoretic transform.
 *  pick prime p such that 
 *  p = c * 2^k + 1, then
 *  ord = 2^k, and
 *  r = g^c, where g is a primitive root
 *  common values: p, r, ord.
 *  7340033, 5, 1 << 20
 *  469762049, 13, 1 << 25
 *  998244353, 31, 1 << 23
 *  1107296257, 8, 1 << 24
 *  ... need __int128 for these
 *  10000093151233, 366508, 1 << 26
 *  1000000523862017, 2127080, 1 << 26
 *  To find solution mod arbitrary modulus, use CRT
 *  ! watch for 64 bit int overflow
 */

struct ntconv {
  ll p, r, rinv, ord;
  ntconv(ll p, ll r, ll ord) : p(p), r(r), rinv(modinv(r, p)), ord(ord) {}
  void ntt(vector<ll>& A, bool inv) {
    ll n = sz(A);
    for (ll i = 1, j = 0; i < n; i++) {
      ll b = n >> 1;
      for (; j & b; b >>= 1) j ^= b;
      j ^= b; if (i < j) swap(A[i], A[j]);
    }
    for (ll l = 2; l <= n; l <<= 1) {
      ll wl = inv ? rinv : r;
      for (ll i = l; i < ord; i <<= 1) wl = wl * wl % p;
      for (ll i = 0; i < n; i += l) {
        ll w = 1;
        for (ll j = 0; j < l/2; j++) {
          ll u = A[i+j], v = A[i+j+l/2] * w % p;
          A[i+j] = u + v < p ? u + v : u + v - p;
          A[i+j+l/2] = u - v >= 0 ? u - v : u - v + p;
          w = w * wl % p;
        }
      }
    }
    if (inv) {
      ll ninv = modinv(n, p);
      for (auto& a : A) a = a * ninv % p;
    }
  }
  vector<ll> mult(vector<ll> A, vector<ll> B) {
    int n = sz(A), m = sz(B), N = 1;
    while (N < n + m) N <<= 1;
    A.resize(N); B.resize(N);
    ntt(A, 0); ntt(B, 0); vector<ll> ans(N);
    for (int i = 0; i < N; i++) ans[i] = A[i] * B[i] % p;
    ntt(ans, 1);
    return ans;
  }
};

