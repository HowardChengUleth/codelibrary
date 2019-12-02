// solve Ax = b
const double eps = 1e-9;
int solve(int n, int m, vector<vector<double>> A, vector<double> b, vector<double>& ans) {
  for (int r = 0, c = 0; r < n and c < m; c++, r++) {
    int pv = r;
    for (int i = r; i < n; i++) if (abs(A[pv][c]) < abs(A[i][c])) pv = i;
    if (abs(A[pv][c]) < eps) return 0;
    swap(A[pv], A[r]); swap(b[pv], b[r]);
    for (int i = 0; i < n; i++) {
      if (i == r) continue;
      double x = A[i][c] / A[r][c];
      b[i] = b[i] - b[r] * x;
      for (int j = 0; j < n; j++) {
        A[i][j] = A[i][j] - A[r][j] * x;
      }
    }
  }
  ans.assign(m, 0);
  for (int r = 0, c = 0; r < n and c < m; r++) {
    while (c < m and abs(A[r][c]) < eps) c++;
    if (c < m) ans[c] = b[r] / A[r][c];
  }
  for (int r = 0; r < n; r++) {
    double sum = 0;
    for (int c = 0; c < m; c++) sum += A[r][c] * ans[c];
    if (abs(sum - b[r]) > eps) return 0;
  }
  return 1;
}

