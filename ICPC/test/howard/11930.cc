// This is basically 2-SAT in disguise.  For each rectangle, there are two
// diagonals to choose from (just one).  For rectangle i, let's call the
// diagonals +i and -i.  If two diagonals d1 and d2 intersect, then choosing
// d1 implies -d2, and choosing d2 implies -d1.  Make a graph with the
// diagonals as vertices, and add an edge if there is an implication from one
// node to another.  Then there is a satisfiable assignment if and only
// if there is no strongly connected component that contains both diagonals
// of a single rectangle.
//
// (This is a well-known result for 2-SAT.  Come and ask me if you want to
//  know more.)
//


#include <iostream>
#include <algorithm>
#include <stack>
#include <cassert>
#include <vector>
using namespace std;


#include <cmath>

struct Point {
  double x, y;
  Point(double xx = 0, double yy = 0) : x(xx), y(yy) { }

  double dist(const Point &p)
  {
    return hypot(x-p.x, y-p.y);
  }
};

typedef pair<int, pair<Point, Point> > Diag;
Diag diag[2000];

istream &operator>>(istream &is, Point &p)
{
  return is >> p.x >> p.y;
}

/* how close to call equal */
const double EPSILON = 1E-8;

double direction(Point p1, Point p2, Point p3)
{
  double x1 = p3.x - p1.x;
  double y1 = p3.y - p1.y;
  double x2 = p2.x - p1.x;
  double y2 = p2.y - p1.y;
  return x1*y2 - x2*y1;
}

int on_segment(Point p1, Point p2, Point p3)
{
  return ((p1.x <= p3.x && p3.x <= p2.x) || (p2.x <= p3.x && p3.x <= p1.x)) &&
    ((p1.y <= p3.y && p3.y <= p2.y) || (p2.y <= p3.y && p3.y <= p1.y));
}

int intersect(const Diag &diag1, const Diag &diag2)
{
  Point a1 = diag1.second.first, a2 = diag1.second.second;
  Point b1 = diag2.second.first, b2 = diag2.second.second;
  
  double d1 = direction(b1, b2, a1);
  double d2 = direction(b1, b2, a2);
  double d3 = direction(a1, a2, b1);
  double d4 = direction(a1, a2, b2);

  if (((d1 > EPSILON && d2 < -EPSILON) || (d1 < -EPSILON && d2 > EPSILON)) &&
      ((d3 > EPSILON && d4 < -EPSILON) || (d3 < -EPSILON && d4 > EPSILON))) {
    return 1;
  } else {
    return (fabs(d1) < EPSILON && on_segment(b1, b2, a1)) ||
      (fabs(d2) < EPSILON && on_segment(b1, b2, a2)) ||
      (fabs(d3) < EPSILON && on_segment(a1, a2, b1)) ||
      (fabs(d4) < EPSILON && on_segment(a1, a2, b2));
  }
}

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

const int MAX_N = 2000;
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
int NOT(int i) { return i + ((i & 1) ? -1 : 1); }

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

bool do_case()
{
  int n;
  cin >> n;
  if (!n) return false;

  Graph G(2*n);

  for (int i = 0; i < n; i++) {
    Point p[4];
    for (int j = 0; j < 4; j++) {
      cin >> p[j];
    }
    double d = p[0].dist(p[1]);
    for (int j = 2; j < 4; j++) {
      double t = p[0].dist(p[j]);
      if (t > d) {
	d = t;
	swap(p[j], p[1]);
      }
    }
    diag[2*i] = make_pair(i, make_pair(p[0], p[1]));
    diag[2*i+1] = make_pair(i, make_pair(p[2], p[3]));
  }

  for (int i = 0; i < 2*n; i++) {
    for (int j = 0; j < 2*n; j++) {
      if (diag[i].first == diag[j].first) continue;
      if (intersect(diag[i], diag[j])) {
	add_clause(G, i, j);
      }
    }
  }

  bool val[1000];
  if (twoSAT(G, val)) {
    cout << "YES" << endl;
  } else {
    cout << "NO" << endl;
  }

  return true;
}

int main(void)
{
  while (do_case())
    ;
  return 0;
}
