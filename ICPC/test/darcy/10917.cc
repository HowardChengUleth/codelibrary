#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <queue>

using namespace std;

struct Edge{ int to,weight; }; // weight can be double or other numeric type

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  
  void add_edge_undirected(int u, int v, int weight) {
    Edge e1 = {v,weight}; nbr[u].push_back(e1);
    Edge e2 = {u,weight}; nbr[v].push_back(e2);
  }
};

void dijkstra(const Graph &G, int src, vector<int> &D, vector<int> &P) {
  typedef pair<int,int> pii;
  int n = G.num_nodes;
  vector<bool> used(n, false);
  priority_queue<pii, vector<pii>, greater<pii> > fringe;
  D.resize(n); fill(D.begin(), D.end(), -1);
  P.resize(n); fill(P.begin(), P.end(), -1);
  D[src] = 0; used[src] = true;
  for (vector<Edge>::const_iterator it = G.nbr[src].begin(); it != G.nbr[src].end(); ++it) {
    int v = it->to; D[v] = it->weight; P[v] = src;
    fringe.push(make_pair(D[v], v));
  }
  while (!fringe.empty()) {
    pii next = fringe.top(); fringe.pop();
    int u = next.second;
    if (used[u]) continue;
    used[u] = true;
    for (vector<Edge>::const_iterator it = G.nbr[u].begin(); it != G.nbr[u].end(); ++it) {
      int v = it->to, weight = it->weight + next.first;
      if (used[v]) continue;
      if (D[v] == -1 || weight < D[v]) {
	D[v] = weight; P[v] = u; fringe.push(make_pair(D[v], v));
      }
    }
  }
}


const int MAX_N = 1010;
int A[MAX_N][MAX_N];
int numPaths[MAX_N];

int paths(int v,int n){
  if(numPaths[v] >= 0)
    return numPaths[v];
  if(v == 2)
    return 1;
  numPaths[v] = 0;
  for(int i=1;i<=n;i++)
    if(A[v][i]){
      numPaths[v] += paths(i,n);
    }
  return numPaths[v];
}

int main(void)
{
  int n,m;
  while (cin >> n >> m && n > 0) {
    Graph G(n+1);
    int u, v, w;
    
    for(int i=1;i<=n;i++)
      for(int j=1;j<=n;j++)
	A[i][j] = 0;
    
    for(int i=0;i<m;i++){
      cin >> u >> v >> w;
      G.add_edge_undirected(u,v,w);
      A[u][v] = A[v][u] = 1;
    }
    
    vector<int> D, P;
    dijkstra(G, 2, D, P);
    
    for(int i=1;i<=n;i++)
      for(int j=1;j<=n;j++)
	A[i][j] = (D[i] > D[j]) && A[i][j];
    
    for(int i=1;i<=n;i++)
      numPaths[i] = -1;
    cout << paths(1,n) << endl;
  }
  return 0;
}
