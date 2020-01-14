/*  Minimum weight perfect matching in O(n^2 m)
 *  where n = #people, m = #tasks and n <= m.
 *  A[i][j] = cost to assign person i task j.
 *  returns the min weight and a vector containing the optimal assignment
 */
template<typename T>
pair<T, vector<int>> hungarian(const vector<vector<T>>& A) {
  int n = sz(A), m = sz(A[0]); T inf = numeric_limits<T>::max() / 2;
  vector<int> way(m + 1), p(m + 1), used(m + 1), ans(n); vector<T> u(n + 1), v(m + 1), minv(m + 1);
  for (int i = 1; i <= n; i++) {
    int j0 = 0, j1 = 0; p[0] = i; minv.assign(m + 1, inf), used.assign(m + 1, 0);
    do {
      int i0 = p[j0]; j1 = 0; T delta = inf; used[j0] = true;
      for (int j = 1; j <= m; j++) if (!used[j]) {
        T cur = A[i0 - 1][j - 1] - u[i0] - v[j];
        if (cur < minv[j]) minv[j] = cur, way[j] = j0;
        if (minv[j] < delta) delta = minv[j], j1 = j;
      }
      for (int j = 0; j <= m; j++) {
        if (used[j]) u[p[j]] += delta, v[j] -= delta;
        else minv[j] -= delta;
      }
    } while (j0 =  j1, p[j0]);
    do { int j1 = way[j0]; p[j0] = p[j1]; j0 = j1; } while (j0);
  }
  for (int i = 1; i <= m; i++) if (p[i] > 0) ans[p[i] - 1] = i - 1;
  return {-v[0], ans};
}

