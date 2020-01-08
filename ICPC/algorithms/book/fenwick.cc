// add(i, v) = add v to A[i] | i in [1, n]
// query(i) = range sum [1, i]
struct fenwick {
  int n; vector<int> A;
  fenwick(int n) : n(n+1), A(n+1) { }
  void add(int i, int v) { while (i < n) A[i] += v, i += i & -i; } 
  int query(int i) { int s = 0; while (i > 0) s += A[i], i -= i & -i; return s; }
};
