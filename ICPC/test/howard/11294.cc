#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

const int MAX_N = 120;

typedef int Edge;


struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  
  // No check for duplicate edges!
  // Add (or remove) any parameters that matter for your problem
  void add_edge_directed(int u, int v) {
    Edge e = v;    nbr[u].push_back(e);
  }
};

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


int VAR(int i) { return 2*i; }
int NOT(int i) { return i ^ 1; }

void add_clause(Graph &G, int v, int w) { // adds (v || w)
  G.add_edge_directed(NOT(v), w);
  G.add_edge_directed(NOT(w), v);
}

bool twoSAT(const Graph &G, bool val[]) {   // assumes graph is built
  SCC(G);
  for (int i = 0; i < G.num_nodes; i += 2) {
    if (comp[i] == comp[i+1]) return false;
    val[i/2] = (comp[i] < comp[i+1]);
  }
  return true;
}



int index(int id, char c)
{
  return VAR(2*id + (c == 'h'));
}

bool solve()
{
  int n, p;
  cin >> n >> p;
  if (!n && !p) return false;

  Graph G(4*n);

  add_clause(G, index(0, 'w'), index(0, 'w'));
  for (int i = 0; i < n; i++) {
    add_clause(G, index(i, 'h'), index(i, 'w'));
    add_clause(G, NOT(index(i, 'h')), NOT(index(i, 'w')));
  }

  for (int i = 0; i < p; i++) {
    int id1, id2;
    char c1, c2;
    cin >> id1 >> c1 >> id2 >> c2;
    add_clause(G, index(id1, c1), index(id2, c2));
  }

  bool val[60];
  if (twoSAT(G, val)) {
    for (int i = 1; i < n; i++) {
      if (i > 1) cout << ' ';
      if (val[index(i, 'h')/2]) {
	cout << i << 'h';
      } else {
	cout << i << 'w';
      }
    }
    cout << endl;
  } else {
    cout << "bad luck" << endl;
  }
  
  return true;
}

int main()
{
  while (solve())
    ;
  return 0;
}
