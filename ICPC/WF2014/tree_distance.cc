// NOTE: For trees only!

// Returns a node that is the furthest from u -- O(n)
int furthest(const Graph& G,int u,int& max_depth,int par[],int p=-1,int d=0){
  if(d == 0 || d > max_depth) max_depth = d;
  int D,v,ans = u;  par[u] = p;
  for(int i=0;i<G.nbr[u].size();i++){
    if(p == G.nbr[u][i]) continue;
    D = max_depth; v = furthest(G,G.nbr[u][i],max_depth,par,u,d+1);
    if(max_depth > D) ans = v;
  }
  return ans;
}

// The eccentricity of u is the distance to the furthest away node from u -- O(n)
int eccentricity(const Graph& G,int u){
  int max_d,p[MAX_N]; furthest(G,u,max_d,p); return max_d;
}

// The diameter of G is the maximum distance between two nodes -- O(n)
int diam(const Graph& G){
  int max_d,p[MAX_N]; furthest(G,furthest(G,0,max_d,p),max_d,p); return max_d;
}

// The center of G is/are the node(s) with minimum eccentricity -- O(n)
//    (.second == -1 if there is only one center)
pii center(const Graph& G){
  int max_d,v,p[MAX_N]; v = furthest(G,furthest(G,0,max_d,p),max_d,p);
  for(int i=0;i<max_d/2;i++,v = p[v]);
  return make_pair(v,(max_d % 2 ? p[v] : -1));
}
