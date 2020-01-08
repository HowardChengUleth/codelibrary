struct matching {
  int l, r, p; vector<int> M, U, D; vector<vector<int>> A; queue<int> Q;
  matching (int l, int r) : l(l), r(r), D(r+1), A(r) {}
  void add_edge(int u, int v) { A[v].push_back(u); }
  bool bfs() {
    for (int v = 0; v < r; v++) if (!U[v]) D[v] = p, Q.push(v);
    while (!Q.empty()) {
      int v = Q.front(); Q.pop();
      if (D[v] != D[r]) for (int u : A[v]) if (D[M[u]] < p)
        D[M[u]] = D[v] + 1, Q.push(M[u]);
    }
    return D[r] >= p;
  }
  int dfs(int v) {
    if (v == r) return 1;
    for (int u : A[v]) if (D[M[u]] == D[v] + 1 and dfs(M[u])) 
      return M[u] = v, 1;
    D[v] = D[r]; return 0;
  }
  pair<int, vector<int>> match() {
    int res = 0; M.assign(l, r), U.assign(r+1, 0);
    for (p = 0; bfs(); p = D[r] + 1) for (int v = 0; v < r; v++)
      if (!U[v] and dfs(v)) U[v] = 1, res++;
    replace(all(M), r, -1); return {res, M};
  }
};

