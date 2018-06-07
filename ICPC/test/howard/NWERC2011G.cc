#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <climits>

using namespace std;

const double EPS = 1e-9;

bool dEqual(double x,double y) { return fabs(x-y) < EPS; }

struct Point {
  double x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(double r,Point p){ p.x *= r; p.y *= r; return p; }
double operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
double len(Point p){ return sqrt(p*p); }
double cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }

struct Edge{ int to; double weight; }; // weight may be any data-type

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }
  
  // Does not allow for duplicate edges between u and v.
  //    (Note that if "typedef int Edge;", do not write the ".to")
  void add_edge(int u, int v, double weight) {
    for(int i=0;i<nbr[u].size();i++)
      if(nbr[u][i].to == v) {
        // An edge between u and v is already here.
        // Add tie breaking here if necessary (for example, keep the smallest cost).
        nbr[u][i].weight = min(nbr[u][i].weight, weight);
        return;
      } 
    Edge e = {v,weight};    nbr[u].push_back(e);
  }

  void add_edge_dup(int u, int v, double weight) {
    Edge e = {v,weight};    nbr[u].push_back(e);
  }

  void del_last_edge(int u) {
    nbr[u].pop_back();
  }

};

#include <cfloat>

bool bellmanford(const Graph& G, int src, double D[], int P[]){
  int n = G.num_nodes;
  fill_n(D,n,DBL_MAX); fill_n(P,n,-1);
  D[src] = 0;
  for (int k = 0; k < n-1; k++)
    for (int v = 0; v < n; v++)
      for (int w = 0; D[v] != DBL_MAX && w < G.nbr[v].size(); w++) {
        Edge p = G.nbr[v][w];
        if (D[p.to] == DBL_MAX || D[p.to] > D[v] + p.weight) {
          D[p.to] = D[v] + p.weight; P[p.to] = v;
        } else if (D[p.to] == D[v] + p.weight) { } // tie-breaking
      }
  
  for (int v = 0; v < n; v++) // negative cycle detection
    for (int w = 0; w < G.nbr[v].size(); w++)
      if (D[v] != INT_MAX) {
        Edge p = G.nbr[v][w];
        if (D[p.to] == DBL_MAX || D[p.to] > D[v] + p.weight)
	  { return false; } // Found a negative cycle
      }
  return true;
}


void solve()
{
  int n, m;
  cin >> n >> m;

  string name[101];
  bool used[101] = {false};
  Point loc[101];
  
  for (int i = 1; i <= n; i++) {
    cin >> name[i] >> loc[i].x >> loc[i].y;
  }

  double dist[101][101];
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      dist[i][j] = len(loc[i] - loc[j]);
    }
  }

  Graph G(n+1);
  for (int k = 0; k < m; k++) {
    string S1, S2, S3, junk;
    cin >> S1 >> junk >> S2 >> junk >> junk >> S3;
    int p1 = find(name, name+n, S1) - name;
    int p2 = find(name, name+n, S2) - name;
    int p3 = find(name, name+n, S3) - name;

    used[p2] = used[p3] = true;
    
    // var2 + dist[p1][p2] < var3 + dist[p1][p3]
    // var2 - var3 < dist[p1][p3] - dist[p1][p2]
    G.add_edge(p3, p2, dist[p1][p3] - dist[p1][p2] - 0.9e-7);
  }

  for (int i = 1; i <= n; i++) {
    G.add_edge(0, i, 0);
  }

  double D[101];
  int P[101];
  if (!bellmanford(G, 0, D, P)) {
    cout << "IMPOSSIBLE" << endl;
    return;
  }

  pair<double, string> ans[100];
  int ans_n = 0;
  for (int i = 1; i <= n; i++) {
    if (used[i]) {
      ans[ans_n++] = make_pair(D[i], name[i]);
    }
  }

  sort(ans, ans+ans_n);

  // check if any of the adjacent firings can be swapped
  for (int i = 0; i < ans_n-1; i++) {
    int p1 = find(name, name+n, ans[i].second) - name;
    int p2 = find(name, name+n, ans[i+1].second) - name;

    // var2 < var1 or var2 - var1 < 0
    G.add_edge_dup(p1, p2, -0.9e-7);
    if (bellmanford(G, 0, D, P)) {
      cout << "UNKNOWN" << endl;
      return;
    }
    G.del_last_edge(p1);
  }

  for (int i = 0; i < ans_n; i++) {
    if (i) cout << ' ';
    cout << ans[i].second;
  }
  cout << endl;
}

int main()
{
  int T;
  cin >> T;
  while (T--)
    solve();
  return 0;
}
    
