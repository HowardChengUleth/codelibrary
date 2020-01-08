// segment tree with lazy prop, log(n) range query and range update.
// st.update(l, r, v) -> apply(i, v) where i ranges in [l, r]
// st.query(l, r) -> compute op of range [l, r]
// think about non-commutative ops!!
template<typename T>
struct segment {

  // these will work for min/max query and range add.
  // most other ops will require modification here.
  void apply(int i, int v) {
    S[i] += v;
    D[i] += v;
  }

  void prop(int i) {
    if (depth(i) != d and D[i]) {
      apply(2*i+1, D[i]);
      apply(2*i, D[i]);
      D[i] = 0;
    }
  }

  // initialize tree with size n, op: (T, T) -> (T), identity value and optional initial data.
  int n, d; T id; function<T(T, T)>op; 
  vector<int> L, R, D; vector<T> S;
  int depth(int i) { return 31 - __builtin_clz(i); }
  segment(int n, T id, function<T(T, T)> op, const vector<T>& A = {}) : n(n), d(depth(n) + (n != 1 << depth(n))), 
  id(id), op(op), L(1 << (d+1), 0), R(1 << (d+1), 0), D(1 << (d+1), 0), S(1 << (d+1), id) {
    for (int i = 0; i <= d; i++)
      for (int j = (1 << i); j < (1 << (i+1)); j++)
        L[j] = (j % (1 << i)) * (1 << (d - i)),
        R[j] = L[j] + (1 << (d - i)) - 1;
    for (int i = 0; i < sz(A); i++) S[(1<<d)+i] = A[i];
    for (int i = (1 << d) - 1; i > 0; i--) S[i] = op(S[2*i], S[2*i+1]);
  }
  // update range [l, r]
  void update(int l, int r, int v, int i = 1) {
    if (r < l) return;
    if (L[i] == l and R[i] == r) apply(i, v);
    else {
      prop(i);
      update(l, min(r, R[2*i]), v, 2*i); 
      update(max(l, L[2*i+1]), r, v, 2*i+1);
      S[i] = op(S[2*i], S[2*i+1]);
    }
  }
  // query op in range [l, r]
  T query(int l, int r, int i = 1) {
    if (r < l) return id;
    if (L[i] == l and R[i] == r) return S[i];
    else {
      prop(i);
      return op(query(l, min(r, R[2*i]), 2*i), query(max(l, L[2*i+1]), r, 2*i+1));
    }
  }
};

// example
int n = 1 << 20;
vector<int> A(n, 0);
segment<int> stmin(n, 1 << 30, [] (int a, int b) { return min(a, b); }, A);
segment<int> stmax(n, -(1 << 30), [] (int a, int b) { return max(a, b); }, A);

