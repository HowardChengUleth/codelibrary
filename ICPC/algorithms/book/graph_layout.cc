// Graph layout
//  -- Each problem has its own Edge structure.
// If you see "typedef int Edge;" at the top of an algorithm, change
//    vector<vector<Edge> > nbr; --->  vector<vector<int> > nbr;

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  
  // No check for duplicate edges!
  // Add (or remove) any parameters that matter for your problem
  void add_edge_directed(int u, int v, int weight, double cost, ...) {
    Edge e = {v,weight,cost, ...};    nbr[u].push_back(e);
  }
  void add_edge_undirected(int u, int v, int weight, double cost, ...) {
    Edge e1 = {v,weight,cost, ...};   nbr[u].push_back(e1);
    Edge e2 = {u,weight,cost, ...};   nbr[v].push_back(e2);
  }

  // Does not allow for duplicate edges between u and v.
  //    (Note that if "typedef int Edge;", do not write the ".to")
  void add_edge_directed_no_dup(int u, int v, int weight, double cost, ...) {
    for(int i=0;i<nbr[u].size();i++) {
      if(nbr[u][i].to == v) {
        // An edge between u and v is already here.
        // Add tie breaking here if necessary (for example, keep the smallest cost).
        nbr[u][i].cost = min(nbr[u][i].cost,cost);
        return;
      } 
    }
    Edge e = {v,weight,cost, ...};    nbr[u].push_back(e);
  }
  void add_edge_undirected_no_dup(int u, int v, int weight, double cost, ...) {
    add_edge_directed_no_dup(u,v,weight,cost, ...);
    add_edge_directed_no_dup(v,u,weight,cost, ...);
  }
};

// Get path from (src) to (v). Stored in path[0], .. ,path[k-1]
int get_path(int v, int P[], int path[]) {
  int k = 0;
  path[k++] = v;
  while (P[v] != -1) path[k++] = v = P[v];
  reverse(path,path+k);
  return k;
}
