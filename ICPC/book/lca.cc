// include RMQ code (Minimum) -- MAX_N must be 2*MAX_NODES
//   call constructLCA once before starting      O(n)
//   call LCA to find lca of vertex u and v      O(log n)
typedef pair<int,int> Type;             // These two lines replace the
const Type oo = make_pair(INT_MAX,-1);  // corresponding  lines in rmq.cc
typedef int Edge;

void preLCA(const Graph& G,int r,int p,Type A[MAX_N],int loc[MAX_N],int d,int& idx){
  for(int i=0;i<G.nbr[r].size();i++){ int w = G.nbr[r][i];
    if(w != p) { A[idx++] = make_pair(d,r); preLCA(G,w,r,A,loc,d+1,idx); } }
  loc[r] = idx; A[idx++] = make_pair(d,r);
}
void constructLCA(const Graph& G,int root,pti M[4*MAX_N],int loc[MAX_N]){
  Type A[MAX_N]; int idx=0; preLCA(G,root,-1,A,loc,0,idx);
  constructRMQ(A,M,idx);
}
int LCA(pti M[4*MAX_N],int loc[MAX_N],int u,int v){
  return getmin(M,min(loc[u],loc[v]),max(loc[u],loc[v])).first.second;
}
