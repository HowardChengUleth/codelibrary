struct SCC {
  int n, c;
  vector<vector<int>> G, H;
  vector<int> ord, comp;
  vector<bool> V;
  SCC(int n) : n(n), G(n), H(n) { };
  void add_edge(int u, int v) {
    G[u].push_back(v);
    H[v].push_back(u);
  }
  void dfs1(int v) { 
    V[v] = true;
    for (auto& u : G[v])
      if (!V[u]) dfs1(u);
    ord.push_back(v);
  }
  void dfs2(int v) {
    comp[v] = c;
    for (auto& u : H[v])
      if (comp[u] == -1) dfs2(u);
  }
  vector<int> scc() {
    V.assign(n, 0);
    for (int i = 0; i < n; i++)
      if (!V[i]) dfs1(i);
    comp.assign(n, -1); c = 0;
    for (int i = 0; i < n; i++) {
      int v = ord[n-1-i];
      if (comp[v] == -1) dfs2(v), c++;
    }
    return comp;
  }
  vector<vector<int>> dag() {
    set<pair<int, int>> S;
    vector<vector<int>> dag(c);
    for (int a = 0; a < n; a++) {
      for (auto& b : G[a]) {
        if (comp[a] == comp[b]) continue;
        if (!S.count({comp[a], comp[b]})) {
          dag[comp[a]].push_back(comp[b]);
          S.insert({comp[a], comp[b]});
        }
      }
    }
    return dag;
  }
};

