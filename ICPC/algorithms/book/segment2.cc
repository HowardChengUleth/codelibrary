// segment tree range query with point updates
template<typename T>
struct segment {
  // initialize tree with size n, op: (T, T) -> (T), identity value and optional initial data.
  int n, d; T id; function<T(T, T)> op;
  vector<int> L, R;
  vector<T> S;
  int depth(int i) { return 31 - __builtin_clz(i); }
  segment(int n, T id, function<T(T, T)> op, const vector<T>& A = {}) : n(n), d(depth(n) + (n != 1 << depth(n))), 
  id(id), op(op), L(1 << (d+1), 0), R(1 << (d+1), 0), S(1 << (d+1), id) {
    for (int i = 0; i <= d; i++)
      for (int j = (1 << i); j < (1 << (i+1)); j++)
        L[j] = (j % (1 << i)) * (1 << (d - i)),
        R[j] = L[j] + (1 << (d - i)) - 1;
    for (int i = 0; i < sz(A); i++) S[(1<<d)+i] = A[i];
    for (int i = (1 << d) - 1; i > 0; i--) S[i] = op(S[2*i], S[2*i+1]);
  }
  // update A[k] to v
  void update(int k, T v, int i = 1) {
    if (k < L[i] or R[i] < k) return;
    if (L[i] == k and k == R[i]) S[i] = v;
    else {
      set(k, v, 2*i); 
      set(k, v, 2*i+1);
      S[i] = op(S[2*i], S[2*i+1]);
    }
  }
  // query range A[l], ... , A[r].
  T query(int l, int r, int i = 1) {
    if (r < l) return id;
    if (L[i] == l and R[i] == r) return S[i];
    else return op(query(l, min(r, R[2*i]), 2*i), query(max(l, L[2*i+1]), r, 2*i+1));
  }
};

// examples
int n = 16;
vector<int> A(n, 1);
segment<int> stmin(n, 1<<30, [] (int a, int b) { return min(a, b); }, A);
segment<int> stmax(n, -(1<<30), [] (int a, int b) { return max(a, b); }, A);
segment<int> stadd(n, 0, [] (int a, int b) { return a + b; }, A);

