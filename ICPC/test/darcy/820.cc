// 
// Network Flow (Ford-Fulkerson Implementation)
//
// Author: Darcy Best
// Source: Introduction to Algorithms by CLRS
//
// This implementation has a runtime of O(E*f) where f is the maximum flow

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
using namespace std;

#include <iostream>
#include <vector>
using namespace std;

struct Edge;
typedef vector<Edge>::iterator EdgeIter;

struct Edge {
  int to, cap, flow;
  bool is_real;
  pair<int,int> part;
  EdgeIter partner;
  
  int residual() const { return cap - flow; }
};

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }

  void add_edge_directed(int u,int v,int cap,int flow,bool is_real,pair<int,int> part){
    Edge e = {v,cap,flow,is_real,part}; nbr[u].push_back(e);
  }
};


// Use this instead of G.add_edge_directed in your actual program
void add_edge(Graph& G,int u,int v,int cap){
  int U = G.nbr[u].size(), V = G.nbr[v].size();
  G.add_edge_directed(u,v,cap,0,true ,make_pair(v,V));
  G.add_edge_directed(v,u,0  ,0,false,make_pair(u,U));
}

void push_path(Graph& G, int s, int t, const vector<EdgeIter>& path, int flow) {
  for (int i = 0; s != t; s = path[i++]->to)
    if (path[i]->is_real) {
      path[i]->flow += flow;    path[i]->partner->cap += flow;
    } else {
      path[i]->cap -= flow;     path[i]->partner->flow -= flow;
    }
}

int augmenting_path(Graph& G, int s, int t, vector<EdgeIter>& path,
		    vector<bool>& visited, int step = 0) {
  if (s == t) return -1;  visited[s] = true;
  for (EdgeIter it = G.nbr[s].begin(); it != G.nbr[s].end(); ++it) {
    int v = it->to;
    if (it->residual() > 0 && !visited[v]) {
      path[step] = it;
      int flow = augmenting_path(G, v, t, path, visited, step+1);
      if (flow == -1)    return it->residual();
      else if (flow > 0) return min(flow, it->residual());
    }
  }
  return 0;
}

int network_flow(Graph& G, int s, int t) { // note that the graph is modified
  for(int i=0;i<G.num_nodes;i++)
    for(EdgeIter it=G.nbr[i].begin(); it != G.nbr[i].end(); ++it)
      G.nbr[it->part.first][it->part.second].partner = it;
  
  vector<EdgeIter> path(G.num_nodes);
  int flow = 0, f;
  do {
    vector<bool> visited(G.num_nodes, false);
    if ((f = augmenting_path(G, s, t, path, visited)) > 0) {
      push_path(G, s, t, path, f);    flow += f;
    }
  } while (f > 0);
  return flow;
}

int main(){
  int nNodes,nEdges,s,t,u,v,c;
  int C=1;
  while(cin >> nNodes && nNodes){
    Graph G(nNodes);
    cin >> s >> t >> nEdges; s--; t--;
    for(int i=0;i<nEdges;i++){
      cin >> u >> v >> c; u--; v--;
      add_edge(G,u,v,c);
      add_edge(G,v,u,c);
    }
    cout << "Network " << C++ << endl;
    cout << "The bandwidth is " << network_flow(G,s,t) << "." << endl << endl;
  }
  
  return 0;
}
