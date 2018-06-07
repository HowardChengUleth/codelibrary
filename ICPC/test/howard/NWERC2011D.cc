#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <map>

using namespace std;

typedef int Edge;

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  
  // Does not allow for duplicate edges between u and v.
  //    (Note that if "typedef int Edge;", do not write the ".to")
  void add_edge_directed(int u, int v) {
    for(unsigned int i=0;i<nbr[u].size();i++)
      if(nbr[u][i] == v) {
        return;
      } 
    Edge e = v;    nbr[u].push_back(e);
  }
};

// each cell has at most 4 edges, *2 for the negated vars.  Some are double
// counted except the ones on the outside
const int MAX_N = 500*500*4 + 4*500;

int po[MAX_N],comp[MAX_N],num_scc,dfn;

void DFS(const Graph& G,int v,stack<int>& P,stack<int>& S){
  po[v] = dfn++;
  S.push(v);  P.push(v);
  for(unsigned int i=0;i<G.nbr[v].size();i++){
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


typedef pair<int,int> pii;

int m, n;

int p_index(int i1, int j1, int i2, int j2, map<pii, int> &var_i)
{
  int i = i1*(m+2)+j1, j = i2*(m+2)+j2;
  if (i > j) swap(i, j);
  pii p(i, j);
  if (var_i.find(p) != var_i.end()) {
    return var_i[p];
  }
  int s = var_i.size();
  return var_i[p] = s;
}

bool val[MAX_N/2];

bool solve()
{
  cin >> n >> m;

  string A[502];
  for (int i = 0; i < n+2; i++) {
    A[i] = string(m+2, '.');
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      cin >> A[i][j];
    }
  }

  Graph G(n*m*4 + 2*m + 2*n);
  map<pii, int> var_i;

  int b_count = 0, w_count = 0;

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      if (A[i][j] == '.') continue;

      int up = VAR(p_index(i, j, i-1, j, var_i)),
	down = VAR(p_index(i, j, i+1, j, var_i)),
	left = VAR(p_index(i, j, i, j-1, var_i)),
	right = VAR(p_index(i, j, i, j+1, var_i));

      if (A[i][j] == 'B') {
	b_count++;
	
	// first make sure that there are at least 1 'W' neighbour in vertical
	// and horizontal direction
	vector<int> horiz, vert;

	if (A[i-1][j] == 'W') vert.push_back(up);
	if (A[i+1][j] == 'W') vert.push_back(down);
	if (A[i][j-1] == 'W') horiz.push_back(left);
	if (A[i][j+1] == 'W') horiz.push_back(right);
	  
	if (vert.size() == 0 || horiz.size() == 0) return false;

	// make sure we are not connected to both vertical or both horizontal
	if (vert.size() > 1) {
	  add_clause(G, NOT(vert[0]), NOT(vert[1]));
	}
	if (horiz.size() > 1) {
	  add_clause(G, NOT(horiz[0]), NOT(horiz[1]));
	}

	// make sure we connect to at least one vertical and one horizontal
	add_clause(G, vert[0], vert[vert.size()-1]);
	add_clause(G, horiz[0], horiz[horiz.size()-1]);
      } else {      // white cell
	w_count++;
	
	vector<int> black;
	
	if (A[i-1][j] == 'B') black.push_back(up);
	if (A[i+1][j] == 'B') black.push_back(down);
	if (A[i][j-1] == 'B') black.push_back(left);
	if (A[i][j+1] == 'B') black.push_back(right);
	
	// make sure that the white is connected to at most one black
	for (unsigned int s = 0; s < black.size(); s++) {
	  for (unsigned int t = s+1; t < black.size(); t++) {
	    add_clause(G, NOT(black[s]), NOT(black[t]));
	  }
	}

	// no need to make sure white is used at least once, because
	// the black/white counts will ensure that.
      }
    }
  }
  return (w_count == 2*b_count && twoSAT(G, val));
}

int main()
{
  int T;
  cin >> T;
  while (T--)
    cout << (solve() ? "YES" : "NO") << endl;
  
  return 0;
}
