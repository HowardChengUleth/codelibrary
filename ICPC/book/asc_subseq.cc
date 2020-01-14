int asc_seq(int A[], int n, int S[]) {
  vector<int> last(n+1), pos(n+1), pred(n);
  if (n == 0) return 0;
  int len = 1;                  last[1] = A[pos[1] = 0];
  for (int i = 1; i < n; i++) {
    int j = upper_bound(last.begin()+1, last.begin()+len+1, A[i]) -
      last.begin();      // use lower_bound for strict increasing subsequence
    pred[i] = (j-1 > 0) ? pos[j-1] : -1;
    last[j] = A[pos[j] = i];    len = max(len, j);
  }
  int start = pos[len];
  for (int i = len-1; i >= 0; i--) {  S[i] = A[start];  start = pred[start];  }
  return len;
}
