template<typename F, typename C>
struct mcmf {
  struct edge { int from, to; F flow, cap; C cost; };
  const C inf = numeric_limits<C>::max(); vector<C> pi, dist;
  int n, m; vector<int> pred, pe; vector<vector<int>> g; vector<edge> edges;
  typedef pair<C, int> pci; priority_queue<pci, vector<pci>, greater<pci>> q;
  mcmf(int n) : n(n), m(0), pred(n), pe(n), g(n) {}
  int add_edge(int u, int v, F cap, C cost) {
    edges.push_back({u, v, 0, cap, cost}), g[u].push_back(m++);
    edges.push_back({u, v, 0, 0, -cost}), g[v].push_back(m++);
    return m - 2;
  }
  void bellman_ford() {
    pi.assign(n, 0);
    for (int i = 0; i < n; i++) for (auto& e : edges) if (e.flow < e.cap)
      pi[e.to] = min(pi[e.to], pi[e.from] + e.cost);
  } 
  bool dijkstra(int s, int t) {
    dist.assign(n, inf); pred.assign(n, -1); dist[s] = 0; q.push({0, s});
    while (!q.empty()) {
      auto [d, u] = q.top(); q.pop();
      if (dist[u] == d) for (int i : g[u]) {
        auto& e = edges[i], v = e.to; C rcost = e.cost + pi[u] - pi[v];
        if (e.flow < e.cap and dist[u] + rcost < dist[v])
          pred[v] = u, pe[v] = i, dist[v] = dist[u] + rcost, q.push({dist[u] + rcost, v});
      }
    }
    for (int v = 0; v < n; v++) if (pred[v] != -1) pi[v] += dist[v];
    return dist[t] < inf;
  }
  pair<F, C> augment(int s, int t, F cap) {
    F flow = cap; C cost = 0;
    for (int v = t; v != s; v = pred[v])
      flow = min(flow, edges[pe[v]].cap - edges[pe[v]].flow);
    for (int v = t; v != s; v = pred[v])
      edges[pe[v]].flow += flow, edges[pe[v]^1].flow -= flow, cost += edges[pe[v]].cost * flow;
    return {flow, cost};
  }
  pair<F, C> flow(int s, int t, F cap) {
    for (auto& e : edges) e.flow = 0;
    F flow = 0; C cost = 0; bellman_ford();
    while (flow < cap and dijkstra(s, t)) {
      auto res = augment(s, t, cap - flow); flow += res.fst, cost += res.snd;
    }
    return {flow, cost};
  }
};

