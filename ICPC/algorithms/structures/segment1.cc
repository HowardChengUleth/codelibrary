// found on codeforces blog
// short non-recursive implementation.
template<typename T>
struct segment {
  int n; T id; function<T(T, T)> op;
  vector<T> S;
  segment(int n, T id, function<T(T, T)> op, const vector<T>& A = {}) 
    : n(n), id(id), op(op), S(2*n, id) {
    for (int i = 0; i < sz(A); i++) S[n+i] = A[i];
    for (int i = n-1; i > 0; i--) S[i] = op(S[2*i], S[2*i+1]);
  }
  // add v to A[x]
  void add(int x, T v) {
    for (S[x += n] += v; x > 1; x /= 2) 
      S[x/2] = op(S[x], S[x^1]);
  }
  // set A[x] to v
  void set(int x, T v) {
    for (S[x += n] = v; x > 1; x /= 2) 
      S[x/2] = op(S[x], S[x^1]);
  }
  // query range A[l], ... , A[r-1].
  T query(int l, int r) {
    int ans = id;
    for (l += n, r += n; l < r; l /= 2, r /= 2) {
      if (l & 1) ans = op(ans, S[l++]);
      if (r & 1) ans = op(ans, S[--r]);
    }
    return ans;
  }
};

// examples
int n = 7;
vector<int> A(n, 1);
segment<int> stmin(n, 1<<30, [] (int a, int b) { return min(a, b); }, A);
segment<int> stmax(n, -(1<<30), [] (int a, int b) { return max(a, b); }, A);
segment<int> stadd(n, 0, [] (int a, int b) { return a + b; }, A);

