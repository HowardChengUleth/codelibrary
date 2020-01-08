// croot = root of centroid tree
// par[v] = parent of v in centroid tree
// cadj[v] = decendants of v in centroid tree
struct Centroid {
  int n, cnt = 0, croot; vector<vector<int>> adj, cadj; vector<int> par, mark, size;
  Centroid(int n) : n(n), adj(n), cadj(n), par(n, -1), mark(n), size(n) {}
  void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
  int dfs(int u, int p) {
    size[u] = 1;
    for (int v : adj[u]) if (v != p and !mark[v]) dfs(v, u), size[u] += size[v];
    return size[u];
  }
  int find_centroid(int u, int p, int s) {
    for (int v : adj[u]) if (v != p and !mark[v]) 
      if (size[v] * 2 > s) return find_centroid(v, u, s);
    return u;
  }
  int find_centroid(int src) { return find_centroid(src, -1, dfs(src, -1)); }
  int decompose(int src = 0) {
    int c = find_centroid(src); mark[c] = 1;
    for (int u : adj[c]) if (!mark[u]) {
      int v = decompose(u);
      cadj[c].push_back(v), par[v] = c;
    }
    return croot = c;
  }
};

