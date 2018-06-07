// Min Cost Max Flow for Dense graphs
// cap[i][j] is the capacity, cost[i][j] >= 0 is the cost/unit (**directed!**)
// returns maximum flow, fcost = min cost for max flow, fnet contains flow network.
// O(min(n^2 * flow, n^3*fcost)), cap[i][j] = 0 if edge is not there
const int NN = 1024; // the maximum number of vertices + 1
int cap[NN][NN], cost[NN][NN], fnet[NN][NN], adj[NN][NN], deg[NN];
int par[NN], d[NN], pi[NN];
const int Inf = INT_MAX/2;

#define Pot(u,v) (d[u] + pi[u] - pi[v])
bool dijkstra(int n, int s, int t) {
  for (int i = 0; i < n; i++) {
    d[i] = Inf;   par[i] = -1;
  }
  
  d[s] = 0;     par[s] = -n - 1;

  while (1) {
    int u = -1, bestD = Inf;
    for (int i = 0; i < n; i++) 
      if (par[i] < 0 && d[i] < bestD) bestD = d[u = i];
    if (bestD == Inf) break;

    par[u] = -par[u] - 1;
    for (int i = 0; i < deg[u]; i++) {
      int v = adj[u][i];
      if (par[v] >= 0) continue;
      if (fnet[v][u] && d[v] > Pot(u,v) - cost[v][u]) {
	d[v] = Pot( u, v ) - cost[v][u];     par[v] = -u-1;
      }
      if (fnet[u][v] < cap[u][v] && d[v] > Pot(u,v) + cost[u][v]) {
	d[v] = Pot(u,v) + cost[u][v];        par[v] = -u - 1;
      }
    }
  }
  
  for (int i = 0; i < n; i++) 
    if (pi[i] < Inf) pi[i] += d[i];
  
  return par[t] >= 0;
}
  
#undef Pot
int mcmf( int n, int s, int t, int &fcost ){
  fill(deg, deg+NN, 0);
  for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) 
      if (cap[i][j] || cap[j][i]) adj[i][deg[i]++] = j;
  
  for (int i = 0; i < NN; i++)  fill(fnet[i], fnet[i]+NN, 0);
  fill(pi, pi+NN, 0);
  int flow = fcost = 0;
  
  while (dijkstra(n, s, t)) {
    int bot = INT_MAX;
    for (int v = t, u = par[v]; v != s; u = par[v = u]) 
      bot = min(bot, fnet[v][u] ? fnet[v][u] : (cap[u][v] - fnet[u][v]));

    for (int v = t, u = par[v]; v != s; u = par[v = u]) 
      if (fnet[v][u]) {	fnet[v][u] -= bot; 	fcost -= bot * cost[v][u]; }
      else {fnet[u][v] += bot;      fcost += bot * cost[u][v]; }
    
    flow += bot;
  }

  for (int u = 0; u < NN; u++) 
    for (int v = u; v < NN; v++) {
      int diff = fnet[v][u] - fnet[u][v];
      if (diff > 0) {        fnet[v][u] = diff;   fnet[u][v] = 0; }
      else          {        fnet[u][v] = -diff;  fnet[v][u] = 0; }
    }

  return flow;
}
