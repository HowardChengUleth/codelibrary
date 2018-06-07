// Min Cost Max Flow for Sparse Graph
// O(min((n+m)*log(n+m)*flow, n*(n+m)*log(n+m)*fcost))

struct Edge;
typedef vector<Edge>::iterator EdgeIter;
typedef pair<int,int> pii;
const int oo = INT_MAX / 2;

struct Edge {
  int to, cap, flow, cost;
  bool is_real;
  pair<int,int> part;
  EdgeIter partner;
  
  int residual() const { return cap - flow; }
};

// Use this instead of G.add_edge_directed in your actual program
void add_edge_with_capacity_directed(Graph& G,int u,int v,int cap,int cost){
  int U = G.nbr[u].size(), V = G.nbr[v].size();
  G.add_edge_directed(u,v,cap,0, cost,true ,make_pair(v,V));
  G.add_edge_directed(v,u,0  ,0,-cost,false,make_pair(u,U));
}

void push_path(Graph& G, int s, int t, const vector<EdgeIter>& path, int flow, int& fcost) {
  for (int i = 0; s != t; s = path[i++]->to){
    fcost += flow*path[i]->cost;
    if (path[i]->is_real) {
      path[i]->flow += flow; path[i]->partner->cap += flow;
    } else {
      path[i]->cap -= flow; path[i]->partner->flow -= flow;
    }
  }
}

int augmenting_path(Graph& G, int s, int t, vector<EdgeIter>& path, vector<int>& pi) {
  vector<int> d(G.num_nodes,oo); vector<EdgeIter> pred(G.num_nodes);
  priority_queue<pii,vector<pii>,greater<pii> > pq;
  d[s] = 0; pq.push(make_pair(d[s],s));
  
  while(!pq.empty()){
    int u = pq.top().second, ud = pq.top().first; pq.pop();
    if(u == t) break; if(d[u] < ud) continue;
    for (EdgeIter it = G.nbr[u].begin(); it != G.nbr[u].end(); ++it) {
      int v = it->to;
      if (it->residual() > 0 && d[v] > d[u] + pi[u] - pi[v] + it->cost) {
        pred[v] = it->partner;  d[v] = d[u] + pi[u] - pi[v] + it->cost;
        pq.push(make_pair(d[v],v));
      }
    }
  }
  if(d[t] == oo) return 0;
  
  int len = 0 , flow = pred[t]->partner->residual();
  for(int v=t;v!=s;v=pred[v]->to){ path[len++] = pred[v]->partner;
    flow = min(flow,pred[v]->partner->residual());
  }
  reverse(path.begin(),path.begin()+len);
  for(int i=0;i<G.num_nodes;i++) if(pi[i] < oo) pi[i] += d[i];
  return flow;
}

int mcmf(Graph& G, int s, int t, int& fcost) { // note that the graph is modified
  for(int i=0;i<G.num_nodes;i++)
    for(EdgeIter it=G.nbr[i].begin(); it != G.nbr[i].end(); ++it)
      G.nbr[it->part.first][it->part.second].partner = it;
  
  vector<int> pi(G.num_nodes, 0); vector<EdgeIter> path(G.num_nodes);
  int flow = 0, f; fcost = 0;
  while((f = augmenting_path(G, s, t, path, pi)) > 0){
    push_path(G, s, t, path, f, fcost);    flow += f;
  }
  return flow;
}
