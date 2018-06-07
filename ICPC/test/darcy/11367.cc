// Dijkstra's Algorithm [Sparse Graphs] (Directed and Undirected) -- O((n+m)*log(n+m))
//  -- Edge weight >= 0.  May use get_path to obtain the path.

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;

struct Edge{ int to,weight; }; // weight can be double or other numeric type

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }

  void add_edge_undirected(int u, int v, int weight) {
    Edge e1 = {v,weight};   nbr[u].push_back(e1);
    Edge e2 = {u,weight};   nbr[v].push_back(e2);
  }
  void add_edge_directed(int u, int v, int weight) {
    Edge e1 = {v,weight};   nbr[u].push_back(e1);
  }
};

int dijkstra(const Graph &G, int src, int dest,vector<int> &D) {
  typedef pair<int,int> pii;
  int n = G.num_nodes;
  vector<bool> used(n, false);
  priority_queue<pii, vector<pii>,  greater<pii> > fringe;

  D.resize(n); fill_n(D.begin(),n,-1);

  if(src/101 == dest) return 0;
  
  D[src] = 0;   used[src] = true;
  for (vector<Edge>::const_iterator it = G.nbr[src].begin(); it != G.nbr[src].end(); ++it) {
    int v = it->to;   D[v] = it->weight;
    fringe.push(make_pair(D[v], v));
  }

  while (!fringe.empty()) {
    pii next = fringe.top();    fringe.pop();
    int u = next.second;
    if (used[u]) continue;
    used[u] = true;
    if(u/101 == dest) { return next.first; }
    
    for (vector<Edge>::const_iterator it = G.nbr[u].begin(); it != G.nbr[u].end(); ++it) {
      int v = it->to, weight = it->weight + next.first;
      if(it->weight == -1) continue; // Not really an edge...
      if (used[v]) continue;
      if (D[v] == -1 || weight < D[v]) {
	D[v] = weight;    fringe.push(make_pair(D[v], v));
      }
    }
  }
  return -1;
}

const int MAX_N = 1030;
int P[MAX_N];

void do_query(Graph& G,int n){
  int c,s,e;
  cin >> c >> s >> e;

  // Do not allow me to fill over c in any city...
  if(c != 100)
    for(int i=0;i<n;i++)
      G.nbr[i*101+c][0].weight = -1;
  
  vector<int> D;
  int t = dijkstra(G,s*101,e,D);
  if(t == -1){ cout << "impossible" << endl; }
  else { cout << t << endl; }

  // Get rid of the capacity limit
  if(c != 100)
    for(int i=0;i<n;i++)
      G.nbr[i*101+c][0].weight = P[i];
}

int main(){
  int n,m,u,v,d;
  cin >> n >> m;
  Graph G(n*101);
  for(int i=0;i<n;i++)
    cin >> P[i];

  // I can fill up 1 unit of fuel at each time...
  for(int i=0;i<n;i++)
    for(int j=0;j<100;j++)
      G.add_edge_directed(i*101+j, i*101+j+1, P[i]);
  
  // Now go between cities!
  for(int i=0;i<m;i++){
    cin >> u >> v >> d;
    for(int j=d;j<=100;j++){
      G.add_edge_directed(u*101+j,v*101+(j-d),0);
      G.add_edge_directed(v*101+j,u*101+(j-d),0);
    }
  }
  
  int q;
  cin >> q;
  while(q--)
    do_query(G,n);
  
  return 0;
}
