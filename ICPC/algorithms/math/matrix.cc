typedef long long ll;
const ll mod = 1e9+9;
// square matrix struct with fast mod exp
struct mat {
  int n; vector<vector<ll>> A;
  mat(int n, ll v) : n(n), A(n, vector<ll>(n, v)) {}
  mat(int n) : n(n), A(n, vector<ll>(n, 0)) { for (int i = 0; i < n; i++) A[i][i] = 1; }
  vector<ll>& operator[](int i) { return A[i]; }
  mat operator*(mat& left) {
    auto& a = *this;
    auto& b = left;
    mat r(n, 0);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++)
          r[i][j] += (a[i][k] * b[k][j]) % mod,
          r[i][j] %= mod;
    return r;
  }
  mat operator^(ll e) {
    auto b = *this;
    mat r(n);
    while (e > 0) {
      if (e & 1) r = r * b, e--;
      else b = b * b, e /= 2;
    }
    return r;
  }
};

