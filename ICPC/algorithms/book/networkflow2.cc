// Network flow (Directed and Undirected) -- O(n^3)
// returns max flow.  Look for positive entries in flow array for the flow.

void push(int graph[MAX_N][MAX_N], int flow[MAX_N][MAX_N],
	  int e[], int u, int v) {
  int cf = graph[u][v] - flow[u][v],  d = (e[u] < cf) ? e[u] : cf;
  flow[u][v] += d;      flow[v][u] = -flow[u][v];
  e[u] -= d;            e[v] += d;
}

void relabel(int graph[MAX_N][MAX_N], int flow[MAX_N][MAX_N],
	     int n, int h[], int u) {
  h[u] = -1;
  for (int v = 0; v < n; v++) 
    if (graph[u][v] - flow[u][v] > 0 && (h[u] == -1 || 1 + h[v] < h[u]))
      h[u] = 1 + h[v];
}

void discharge(int graph[MAX_N][MAX_N], int flow[MAX_N][MAX_N],
	       int n, int e[], int h[], list<int>& NU, 
	       list<int>::iterator &current, int u) {
  while (e[u] > 0) 
    if (current == NU.end()) {
      relabel(graph, flow, n, h, u);
      current = NU.begin();
    } else {
      int v = *current;
      if (graph[u][v] - flow[u][v] > 0 && h[u] == h[v] + 1) 
	push(graph, flow, e, u, v);
      else ++current;
    }
}

int network_flow(int graph[MAX_N][MAX_N], int flow[MAX_N][MAX_N], 
		 int n, int s, int t) {
  int e[MAX_N], h[MAX_N], u, v, oh;
  list<int> N[MAX_N], L;
  list<int>::iterator current[MAX_N], p;

  for (u = 0; u < n; u++) h[u] = e[u] = 0;
  for (u = 0; u < n; u++) 
    for (v = 0; v < n; v++) {
      flow[u][v] = 0;
      if (graph[u][v] > 0 || graph[v][u] > 0) N[u].push_front(v);
    }

  h[s] = n;
  for (u = 0; u < n; u++) {
    if (graph[s][u] > 0) {
      e[u] = flow[s][u] = graph[s][u];
      e[s] += flow[u][s] = -graph[s][u];
    }
    if (u != s && u != t) L.push_front(u);
    current[u] = N[u].begin();
  }

  for (p = L.begin(); p != L.end(); ++p) {
    u = *p;         oh = h[u];
    discharge(graph, flow, n, e, h, N[u], current[u], u);
    if (h[u] > oh) {
      L.erase(p);    L.push_front(u);    p = L.begin();
    }
  }
  
  int maxflow = 0;
  for (u = 0; u < n; u++) 
    if (flow[s][u] > 0) maxflow += flow[s][u];
  return maxflow;
}
