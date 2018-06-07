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
};

void dijkstra(const Graph &G, int src, vector<int> &D, vector<int> &P) {
  typedef pair<int,int> pii;
  int n = G.num_nodes;
  vector<bool> used(n, false);
  priority_queue<pii, vector<pii>,  greater<pii> > fringe;

  D.resize(n);  fill(D.begin(), D.end(), -1);  
  P.resize(n);  fill(P.begin(), P.end(), -1);

  D[src] = 0;   used[src] = true;
  for (vector<Edge>::const_iterator it = G.nbr[src].begin(); it != G.nbr[src].end(); ++it) {
    int v = it->to;   D[v] = it->weight;   P[v] = src;
    fringe.push(make_pair(D[v], v));
  }

  while (!fringe.empty()) {
    pii next = fringe.top();    fringe.pop();
    int u = next.second;
    if (used[u]) continue;
    used[u] = true;

    for (vector<Edge>::const_iterator it = G.nbr[u].begin(); it != G.nbr[u].end(); ++it) {
      int v = it->to, weight = it->weight + next.first;
      if (used[v]) continue;
      if (D[v] == -1 || weight < D[v]) {
	D[v] = weight;    P[v] = u;    fringe.push(make_pair(D[v], v));
      }
    }
  }
}

int main(){
  int T,n,m,s,t,u,v,c,C=1;
  cin >> T;
  while(T--){
    vector<int> D,P;
    cin >> n >> m >> s >> t;
    Graph G(n);
    for(int i=0;i<m;i++){
      cin >> u >> v >> c;
      G.add_edge_undirected(u,v,c);
    }
    dijkstra(G,s,D,P);
    cout << "Case #" << C++ << ": ";
    if(D[t] == -1){ cout << "unreachable" << endl; }
    else { cout << D[t] << endl; }    
  }

  return 0;
}
