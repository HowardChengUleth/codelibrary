// Bellman-Ford (Directed and Undirected) -- O(nm)
//  -- May use get_path to obtain the path.

struct Edge{ int to,weight; }; // weight may be any data-type

void bellmanford(const Graph& G, int src, int D[], int P[]){
  int n = G.num_nodes;
  fill_n(D,n,INT_MAX); fill_n(P,n,-1);
  D[src] = 0;
  for (int k = 0; k < n-1; k++)
    for (int v = 0; v < n; v++)
      for (int w = 0; D[v] != INT_MAX && w < G.nbr[v].size(); w++) {
	Edge p = G.nbr[v][w];
	if (D[p.to] == INT_MAX || D[p.to] > D[v] + p.weight) {
	  D[p.to] = D[v] + p.weight; P[p.to] = v;
	} else if (D[p.to] == D[v] + p.weight) { } // tie-breaking
      }
  
  for (int v = 0; v < n; v++) // negative cycle detection
    for (int w = 0; w < G.nbr[v].size(); w++)
      if (D[v] != INT_MAX) {
	Edge p = G.nbr[v][w];
	if (D[p.to] == INT_MAX || D[p.to] > D[v] + p.weight)
	{ } // Found a negative cycle
      }
}
