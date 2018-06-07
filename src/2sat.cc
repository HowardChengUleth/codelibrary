// 2SAT solver: returns T/F whether it is satisfiable -- O(n+m)
//   - use NOT() to negate a variable (works on negated ones too!)
//   - ALWAYS use VAR() to talk about the non-negated version of the var i
//   - use add_clause to add a clause
//   - one possible satisfying assignment is returned in val[], if
//     it exists
//   - To FORCE i to be true:  add_clause(G,VAR(i),VAR(i));
//   - To implement XOR -- say (i XOR j) :
//      add_clause(G,VAR(i),VAR(j)); add_clause(G,NOT(VAR(i)),NOT(VAR(j)));
//     NOTE: val[] is indexed by i for var i, not by VAR(i)!!!

#include <iostream>
#include <algorithm>
#include <stack>
#include <cassert>
#include <vector>
using namespace std;

const int MAX_VARS = 100;          // maximum number of variables
const int MAX_NODES = 2*MAX_VARS;

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
int num_scc;

void DFS(int v, const Graph& G, int& C, stack<int>& P,stack<int>& S){
  po[v] = C++;
  
  S.push(v);  P.push(v);
  for(unsigned int i=0;i<G.adj[v].size();i++){
    int w = G.adj[v][i];
    if(po[w] == -1){
      DFS(w,G,C,P,S);
    } else if(comp[w] == -1){
      while(!P.empty() && (po[P.top()] > po[w]))
	P.pop();
    }
  }
  if(!P.empty() && P.top() == v){
    while(!S.empty()){
      int t = S.top();
      S.pop();
      comp[t] = num_scc;
      if(t == v)
	break;
    }
    P.pop();
    num_scc++;
  }
}

int SCC(const Graph& G){
  num_scc = 0;
  int C = 1;
  stack<int> P,S;
  fill(po,po+G.numNodes,-1);
  fill(comp,comp+G.numNodes,-1);
  for(int i=0;i<G.numNodes;i++)
    if(po[i] == -1)
      DFS(i,G,C,P,S);
  
  return num_scc;
}


int VAR(int i) { return 2*i; }
int NOT(int i) { return i ^ 1; }

void add_clause(Graph &G, int v, int w) { // adds (v || w)
  if (v == NOT(w)) return;
  G.add_edge(NOT(v), w);
  G.add_edge(NOT(w), v);
}

bool twoSAT(const Graph &G, bool val[]) {   // assumes graph is built
  SCC(G);
  for (int i = 0; i < G.numNodes; i += 2) {
    if (comp[i] == comp[i+1]) return false;
    val[i/2] = (comp[i] < comp[i+1]);
  }
  return true;
}


// Declare this as a global variable if MAX_NODES is large to
//   avoid Runtime Error.
Graph G;

int main(){
  int m,n;
  while(cin >> n >> m && (n || m)){
    G.clear();
    G.numNodes = 2*n;

    for (int i = 0; i < m; i++) {
      cout << "Enter two variables for clause (1 - " << n
	   << "), negative means negated: ";
      int x, y;
      cin >> x >> y;

      int var1 = VAR(abs(x)-1), var2 = VAR(abs(y)-1);
      if (x < 0) var1 = NOT(var1);
      if (y < 0) var2 = NOT(var2);
      add_clause(G, var1, var2);
    }

    bool val[MAX_VARS];
    if (twoSAT(G, val)) {
      for (int i = 0; i < n; i++) {
	cout << val[i] << ' ';
      }
      cout << endl;
    } else {
      cout << "Impossible" << endl;
    }
  }
  return 0;
}
