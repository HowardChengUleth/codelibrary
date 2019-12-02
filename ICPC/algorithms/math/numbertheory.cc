// Number Theory functions
// watch for overflows!!!

ll gcd(ll a, ll b) {
  if (b == 0) return a;
  else return gcd(b, a%b);
}

pair<ll, ll> egcd(ll a, ll b) {
  if (b > a) swap(a, b);
  ll s = 0, t = 1, u = 1, v = 0, q = 0;
  while (b != 0) {
    q = a/b; swap(a, b);
    b = b % a; swap(s, u);
    s = s - u * q; swap(t, v);
    t = t - v * q;
  }
  return {u, v};
}

ll modinv(ll a, ll m) {
  return (egcd(a, m).snd + m) % m;
}

ll fexp(ll x, ll p) {
  ll r = 1;
  while (p > 0) {
    if (p & 1) p -= 1, r = (r * x);
    else p >>= 1, x = (x * x);
  }
  return r;
}

ll fexp(ll x, ll p, ll mod) {
  ll r = 1;
  while (p > 0) {
    if (p & 1) p -= 1, r = (r * x) % mod;
    else p >>= 1, x = (x * x) % mod;
  }
  return r;
}

set<pair<ll, ll>> factor(ll n) {
  set<pair<ll, ll>> S;
  for (ll i = 2, a = 0; i <= n; i++) {
    while (n % i == 0) n /= i, a++;
    if (a > 0) S.insert({i, a}), a = 0;
  }
  return S;
}

set<ll> divisors(ll n) {
  set<ll> S;
  for (ll i = 1; i*i <= n; i++)
    if (n % i == 0) 
      S.insert(i), S.insert(n/i);
  return S;
}

ll phi(ll n) {
  ll ans = 1;
  set<pair<ll, ll>> S = factor(n);
  for (auto& s : S) ans *= (fexp(s.fst, s.snd) - fexp(s.fst, s.snd-1));
  return ans;
}

ll crt(vector<ll>& A, vector<ll>& M, ll n) {
  ll m = accumulate(all(M), 1, multiplies<ll>());
  ll x = 0;
  for (ll i = 0; i < n; i++) {
    ll k = m/M[i];
    ll d = egcd(M[i], k % M[i]).snd;
    x += A[i] * k * d % m;
    x += m;
  }
  return x % m;
}
