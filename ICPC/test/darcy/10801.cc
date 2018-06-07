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

  void add_edge_directed(int u, int v, int weight) {
    Edge e1 = {v,weight};   nbr[u].push_back(e1);
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

const int MAX_N = 130;

// Floor in == 0-99
// Floor out == 100-199

int main(){
  int n,k,T[MAX_N],A[MAX_N];
  string s;
  while(cin >> n >> k){
    Graph G(200);
    vector<int> D,P;
    
    // Floor In and Out
    for(int i=0;i<100;i++)
      G.add_edge_directed(i,100+i,60);
    
    for(int i=0;i<n;i++)
      cin >> T[i];
    getline(cin,s);
    for(int i=0;i<n;i++){
      getline(cin,s);
      stringstream ss(s);
      int m = 0;
      while(ss >> A[m]) m++;

      for(int j=0;j<m;j++)
	for(int k=0;k<m;k++)
	  if(j != k)
	    G.add_edge_directed(A[j]+100,A[k],T[i]*abs(A[j]-A[k]));
    }
    dijkstra(G,100,D,P); // Start at floor 0 going out
    if(k == 0){ cout << 0 << endl; }
    else if(D[k] != -1){ cout << D[k] << endl; }
    else { cout << "IMPOSSIBLE" << endl; }      
  }
  return 0;
}
