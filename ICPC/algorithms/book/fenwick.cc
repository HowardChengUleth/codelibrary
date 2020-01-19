// add(i, v) = add v to A[i] | i in [1, n]
// query(i) = range sum [1, i]
// lower_bound(x) = i such that query(i) < x and query(i+1) >= x
struct fenwick {
  int n; vector<int> A;
  fenwick(int n) : n(n+1), A(n+1) { }
  void add(int i, int v) { while (i < n) A[i] += v, i += i & -i; } 
  int query(int i) { int s = 0; while (i > 0) s += A[i], i -= i & -i; return s; }
  int lower_bound(int x) {
    int i = 0; // NOTE: A[i] >= 0. for this to make sense!
    for (int b = 1 << (31 - __builtin_clz(n)); b; b /= 2)
      if (i+b < n and x > A[i+b]) x -=  A[i+b], i += b;
    return i;
  }
};
