// Compresses a directed graph into its strongly connected components
//
// Author: Darcy Best
// Date  : October 1, 2010
//
// A set of nodes is "strongly connected" if for any pair of nodes in
// the set, there is a path from u to v AND from v to u.
//
// Compressing a graph into its strongly connected components means
// converting each strongly connected component into a super-node.
//
// We then build a "compressed" graph made with the super-nodes.  We
// add an edge in the compressed graph between U and V if there is a
// vertex u in U and v in V such that there was an edge from u to v in
// the original graph. The compressed graph will be a Directed Acyclic
// Graph (DAG), and the list of components will be in REVERSE
// topological order.
//
// If you are only concerned with the number of strongly connected
// components, you do not need to build the graph. See comments below
// on how to remove the SCC graph.
//
// The complexity of this algorithm is O(|V| + |E|).
//

#include <iostream>
#include <algorithm>
#include <stack>
#include <cassert>
#include <vector>
using namespace std;

const int MAX_NODES = 100005;

struct Graph{
  int numNodes;
  vector<int> adj[MAX_NODES];
  void clear(){
    numNodes = 0;
    for(int i=0;i<MAX_NODES;i++)
      adj[i].clear();
  }
  void add_edge(int u,int v){
    if(find(adj[u].begin(),adj[u].end(),v) == adj[u].end())
      adj[u].push_back(v);
  }
};

int po[MAX_NODES],comp[MAX_NODES];

void DFS(int v,const Graph& G,Graph& G_scc,int& C,
	 stack<int>& P,stack<int>& S){
  po[v] = C++;
  
  S.push(v);  P.push(v);
  for(unsigned int i=0;i<G.adj[v].size();i++){
    int w = G.adj[v][i];
    if(po[w] == -1){
      DFS(w,G,G_scc,C,P,S);
    } else if(comp[w] == -1){
      while(!P.empty() && (po[P.top()] > po[w]))
	P.pop();
    }
  }
  if(!P.empty() && P.top() == v){
    while(!S.empty()){
      int t = S.top();
      S.pop();
      comp[t] = G_scc.numNodes;
      if(t == v)
	break;
    }
    G_scc.numNodes++;
    P.pop();
  }
}

int SCC(const Graph& G,Graph& G_scc){
  G_scc.clear();
  int C=1;
  stack<int> P,S;
  fill(po,po+G.numNodes,-1);
  fill(comp,comp+G.numNodes,-1);
  for(int i=0;i<G.numNodes;i++)
    if(po[i] == -1)
      DFS(i,G,G_scc,C,P,S);
  
  // You do not need this if you are only interested in the number of
  //    strongly connected components.
  for(int i=0;i<G.numNodes;i++){
    for(unsigned int j=0;j<G.adj[i].size();j++){
      int w = G.adj[i][j];
      if(comp[i] != comp[w])
	G_scc.add_edge(comp[i],comp[w]);
    }
  }
  
  return G_scc.numNodes;
}

// Declare these as a global variable if MAX_NODES is large to
//   avoid Runtime Error.
Graph G,G_scc;

int main(){
  int u,v,m,n;
  int n_scc;
  while(cin >> n >> m && (n || m)){
    G.clear();
    G.numNodes = n;
    for(int i=0;i<m;i++){
      cin >> u >> v;
      G.add_edge(u,v);
    }
    n_scc = SCC(G,G_scc);
    
    cout << "# of Strongly Connected Components: " << n_scc << endl;
  }
  return 0;
}
