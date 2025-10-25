class FenwickTree{  // All entries must be >= 0 even after decrement
public:             // Every function is O(log n)
  FenwickTree(int n) : N(n), iBM(1), tree(n,0) {
    while (iBM < N) iBM *= 2;
  }
  
  // inc/dec the entry at position idx by val
  void incEntry(int idx, int val) {
    do tree[idx] += val; while(idx && (idx += (idx & (-idx))) < N);
  }
  
  // return the cumulative sum val[0] + val[1] + ... + val[idx]
  int cumulativeSum(int idx) const {
    int sum = tree[0];
    for( ; idx > 0 ; idx &= idx-1) sum += tree[idx];
    return sum;
  }
  
  // return the entry indexed by idx
  int getEntry(int idx) const {
    int val = tree[idx], par = idx & (idx-1);
    if (idx--) for( ; par != idx ; idx &= idx-1) val -= tree[idx];
    return val;
  }

  // return the largest index such that the cumulative frequency is
  // what is given, or -1 if it is not found
  int getIndex(int sum) const {
    if ((sum -= tree[0]) < 0) return -1;
    int idx = 0;
    for(int bM = iBM ; bM != 0 && idx < N-1 ; bM >>= 1)
      if (sum >= tree[idx+bM]) sum -= tree[idx += bM];
    return (sum != 0) ? -1 : min(N-1,idx);
  }
  
private:
  int N, iBM; vector<int> tree;
};

// You can extend this to n-D if you want
class FenwickTree2D{  // All entries must be >= 0 even after decrement
public:               // Every function is O(log^2(n))
  FenwickTree2D(int m,int n) : M(m),N(n),tree(m,vector<int>(n,0)) {} // Array is m x n
  
  // inc/dec the entry at (i,j) by val
  void incEntry(int i, int j, int val) {
    do{
      int idx = j;
      do tree[i][idx] += val; while(idx && (idx += (idx & (-idx))) < M);
    } while(x && (x += (x & (-x))) < N);
  }
  
  // return the sum tree[0][0] + ... + tree[i][j]
  int cumSum(int i,int j) const {
    int sum = tree[0][0];
    for( ; i > 0 ; i &= i-1 )
      for(int idx=j ; idx > 0 ; idx &= idx-1) sum += tree[i][idx];
    return sum;
  }
  
private:
  int M,N; vector<vector<int> > tree;
};
