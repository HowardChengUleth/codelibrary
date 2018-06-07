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

int N,M;

const int delta_i[4] = {-1,1,0,0};
const int delta_j[4] = {0,0,1,-1};

int val(int start,int i,int j){
   return start+i*N+j;
}

bool inRange(int i,int j){
   return (i >= 0 && i < M && j >= 0 && j < N);
}

void do_case(){
   int B,x,y,flow; // Rows/cols
      
   cin >> M >> N >> B;

   int SOURCE = 2*N*M;
   int SINK = SOURCE+1;

   Graph graph(2*N*M+2);
   
   int IN = 0;
   int OUT = N*M;
   
   for(int i=0;i<M;i++)
      for(int j=0;j<N;j++)
	 add_edge(graph,val(IN,i,j),val(OUT,i,j),1); // Each Intersection
   
   for(int i=0;i<M;i++){
      add_edge(graph,val(OUT,i,0),SINK,1);   // Left Column
      add_edge(graph,val(OUT,i,N-1),SINK,1); // Right Column
   }
   for(int j=1;j<N-1;j++){
      add_edge(graph,val(OUT,0,j),SINK,1);   // Bottom Row
      add_edge(graph,val(OUT,M-1,j),SINK,1); // Top Row
   }

   for(int i=0;i<M;i++)
      for(int j=0;j<N;j++)
	 for(int k=0;k<4;k++)
	    if(inRange(i+delta_i[k],j+delta_j[k]))
	       add_edge(graph,val(OUT,i,j),val(IN,i+delta_i[k],j+delta_j[k]),1);
   
   for(int i=0;i<B;i++){
      cin >> x >> y;
      x--; y--;
      add_edge(graph,SOURCE,val(IN,x,y),1);
      
   }

   flow = network_flow(graph, SOURCE, SINK);

      
   if(flow == B){
      cout << "possible" << endl;
   } else {
      cout << "not possible" << endl;
   }
   
}

int main(){
   int T;
   cin >> T;
   while(T--)
      do_case();
   return 0;
}
