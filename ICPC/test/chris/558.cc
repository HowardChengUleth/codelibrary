#include <iostream>
#include <vector>
#include <climits>
#include <utility>
using namespace std;

const int MAX_N = 1010;

struct Edge { int to, weight; };

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  void add_edge_directed(int u, int v, int weight) {
    Edge e = { v, weight }; nbr[u].push_back(e);
  }
};

bool bellmanford(const Graph& G, int src, int D[], int P[]) {
  int n = G.num_nodes;
  fill_n(D, n, INT_MAX); fill_n(P, n, -1);
  D[src] = 0;
  for (int k = 0; k < n-1; ++k)
    for (int v = 0; v < n; ++v)
      for (int w = 0; D[v] != INT_MAX && w < G.nbr[v].size(); ++w) {
	Edge p = G.nbr[v][w];
	if (D[p.to] == INT_MAX || D[p.to] > D[v] + p.weight) {
	  D[p.to] = D[v] + p.weight; P[p.to] = v;
	} else if (D[p.to] == D[v] + p.weight) { }
      }
  for (int v = 0; v < n; ++v)
    for (int w = 0; w < G.nbr[v].size(); ++w)
      if (D[v] != INT_MAX) {
	Edge p = G.nbr[v][w];
	if (D[p.to] == INT_MAX || D[p.to] > D[v] + p.weight)
	  return true;
      }
  return false;
} 
	
int main() {
  int c; cin >> c;
  while (c--) {
    int n, m; cin >> n >> m;
    Graph G(n);
    int D[MAX_N], P[MAX_N];
    for (int i = 0; i < m; ++i) {
      int x, y, t; cin >> x >> y >> t;
      G.add_edge_directed(x, y, t);
    }
    if (bellmanford(G, 0, D, P)) cout << "possible\n";
    else cout << "not possible\n";
  }
  return 0;
}
