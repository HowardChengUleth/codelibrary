// Biconnected Components (Undirected Only) -- O(n+m)
//  -- Some articulation points may be processed multiple times.

typedef int Edge;

int dfn, dfs[MAX_N], back[MAX_N];
bool root_art(const Graph& G,int v,int k,int child){
  if(child > 1) return true;
  for(int i=k+1;i<G.nbr[v].size();i++)
    if(!dfs[G.nbr[v][i]]) return true;
  return false;
}

void do_dfs(const Graph& G, int v, int pred, stack<pair<int,int> > &e_stack){
  int child = 0;
  dfs[v] = back[v] = ++dfn;
  for (int i = 0; i < G.nbr[v].size(); i++) {
    int w = G.nbr[v][i];
    if (dfs[w] < dfs[v] && w != pred) e_stack.push(make_pair(v,w));
    if (!dfs[w]) {
      do_dfs(G, w, v, e_stack);      child++;
      
      if (back[w] >= dfs[v]) {                       // new biconnected component
	pair<int,int> e,E = make_pair(v,w);
	do{
	  e = e_stack.top(); e_stack.pop();          // e belongs to this component
	} while(e != E);
	
	if(pred != -1 || root_art(G,v,i,child)){ }   // v is articulation point
      } else back[v] = min(back[v],back[w]);
    } else back[v] = min(back[v],dfs[w]);
  }
}

void bicomp(const Graph& G) {
  stack<pair<int,int> > e_stack;
  dfn = 0; fill_n(dfs,G.num_nodes,0);
  for (int i=0;i<G.num_nodes;i++) // get rid of loop to process only one component
    if (dfs[i] == 0) do_dfs(G, i, -1, e_stack);
}
