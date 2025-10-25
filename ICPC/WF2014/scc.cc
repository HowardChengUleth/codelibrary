// Strongly Connected Components (Directed Only) -- O(n+m)
//  -- Each vertex's component number is stored in comp[].
//  -- The components are in REVERSE topological order
//  -- Also can construct a DAG of connected components

typedef int Edge;

int po[MAX_N],comp[MAX_N],num_scc,dfn;

void DFS(const Graph& G,int v,stack<int>& P,stack<int>& S){
  po[v] = dfn++;
  S.push(v);  P.push(v);
  for(int i=0;i<G.nbr[v].size();i++){
    int w = G.nbr[v][i];
    if(po[w] == -1) DFS(G,w,P,S);
    else if(comp[w] == -1)
      while(!P.empty() && (po[P.top()] > po[w]))
	P.pop();
  }
  if(!P.empty() && P.top() == v){
    while(!S.empty()){
      int t = S.top();       S.pop();
      comp[t] = num_scc;
      if(t == v) break;
    }
    P.pop();   num_scc++;
  }
}

int SCC(const Graph& G){
  num_scc = dfn = 0;
  stack<int> P,S;
  fill(po,po+G.num_nodes,-1);
  fill(comp,comp+G.num_nodes,-1);
  for(int i=0;i<G.num_nodes;i++)
    if(po[i] == -1) DFS(G,i,P,S);
  return num_scc;
}

// Make sure you call SCC first
Graph get_DAG(const Graph& G){
  Graph G_scc(num_scc);
  for(int i=0;i<G.num_nodes;i++){
    for(int j=0;j<G.nbr[i].size();j++){
      int w = G.nbr[i][j];
      if(comp[i] != comp[w])
	G_scc.add_edge_directed_no_dup(comp[i],comp[w]);
    }
  }
  return G_scc;
}
