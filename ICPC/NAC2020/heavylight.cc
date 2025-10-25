/*  Supports queries on paths in trees. O(log^2(n))
 *  Example code at the bottom (must include segment tree code)
 */
template<typename T> struct HLD {
  int n; vector<int> heavy, head, par, pos, level; vector<T> cost;
  vector<vector<pair<int, T>>> adj;
  HLD(int n) : n(n), heavy(n), head(n), par(n), pos(n), level(n), cost(n), adj(n) { }
  int dfs(int u, int p, int d) {
    int size = 1, max_child = 0 , max_child_id = -1;
    par[u] = p, level[u] = d;
    for (auto& child : adj[u]) if (child.fst != p) {
      cost[child.fst] = child.snd;
      int child_size = dfs(child.fst, u, d+1);
      if (child_size > max_child) max_child = child_size, max_child_id = child.fst;
      size += child_size;
    }
    if (max_child * 2 >= size) heavy[u] = max_child_id;
    return size;
  }
  void add_edge(int u, int v, T cost) {
    adj[u].emplace_back(v, cost), adj[v].emplace_back(u, cost);
  }
  vector<T> decompose(int root = 0) {
    vector<T> val(n); heavy.assign(n, -1); dfs(root, -1, 0); int curpos = 0;
    for (int i = 0, cur = 0; i < n; cur = ++i) {
      if (par[i] == -1 or heavy[par[i]] != i) while (cur != -1) 
        val[curpos] = cost[cur], pos[cur] = curpos++, head[cur] = i, cur = heavy[cur];
    }
    return val;
  }
  template<typename F> void range_query(int u, int v, F query) {
    while (head[u] != head[v]) {
      if (level[head[u]] > level[head[v]]) swap(u, v);
      query(pos[head[v]], pos[v]+1); v = par[head[v]];
    }
    if (u != v) query(min(pos[u], pos[v])+1, max(pos[u], pos[v])+1);
  }
};

int n; cin >> n;
HLD<int> tree(n);
for (int i = 1, a, b, c; i < n; i++) {
  cin >> a >> b >> c; a--; b--;
  tree.add_edge(a, b, c);
}

// example where query(u, v) = sum of weights on edges on path from u to v (easy mod to min/max)
// initialization code...
const int id = 0;
vector<int> init = tree.decompose();
segment<int> s(sz(init), id, [] (int a, int b) { return a + b; }, init);

// use this function in your code...
function<int(int, int)> query = [&] (int u, int v) {
  int ans = id;
  tree.range_query(u, v, [&] (int i, int j) { ans = s.op(ans, s.query(i, j)); });
  return ans;
};

debug(query(0, 5));
