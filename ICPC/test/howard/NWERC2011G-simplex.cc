#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

const double EPS = 1e-9, INF = 1.0/0.0;

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

// simplex: C is (m+1)x(n+1).
// First row obj. function (maximize), next m rows are <= constraints
const int MAX_CONSTRAINTS = 1001;
const int MAX_VARS = 100;
const int MAXM = MAX_CONSTRAINTS + 1, MAXN = MAX_VARS + 1;
double A[MAXM][MAXN];
int basis[MAXM], out[MAXN];

void pivot(int m, int n, int a, int b)
{
  int i, j;
  for (i = 0; i <= m; i++)
    if (i != a)
      for (j = 0; j <= n; j++)
        if (j != b) A[i][j] -= A[a][j] * A[i][b] / A[a][b];
  for (j = 0; j <= n; j++) if (j != b) A[a][j] /= A[a][b];
  for (i = 0; i <= m; i++) if (i != a) A[i][b] = -A[i][b] / A[a][b];
  A[a][b] = 1 / A[a][b];
  swap(basis[a], out[b]);
}

double simplex(int m, int n, double C[][MAXN], double X[])
{
  int i, j, ii, jj;
  for (i = 1; i <= m; i++) copy(C[i], C[i]+n+1, A[i]);
  for (j = 0; j <= n; j++) A[0][j] = -C[0][j];
  for (i = 0; i <= m; i++) basis[i] = -i;
  for (j = 0; j <= n; j++) out[j] = j;
  for (;;) {
    for (i = ii = 1; i <= m; i++)
      if (A[i][n] < A[ii][n] || (A[i][n] == A[ii][n] && basis[i] < basis[ii])) ii = i;
    if (A[ii][n] >= -EPS) break;
    for (j = jj = 0; j < n; j++)
      if (A[ii][j] < A[ii][jj] - EPS ||
          (A[ii][j] < A[ii][jj] - EPS && out[i] < out[j])) jj = j;
    if (A[ii][jj] >= -EPS) return -INF;
    pivot(m, n, ii, jj);
  }
  for (;;) {
    for (j = jj = 0; j < n; j++)
      if (A[0][j] < A[0][jj] || (A[0][j] == A[0][jj] && out[j] < out[jj])) jj = j;
    if (A[0][jj] > -EPS) break;
    for (i=1, ii=0; i <= m; i++)
      if ((A[i][jj]>EPS) &&
          (!ii || (A[i][n]/A[i][jj] < A[ii][n]/A[ii][jj]-EPS) ||
           ((A[i][n]/A[i][jj] < A[ii][n]/A[ii][jj]+EPS) &&
            (basis[i] < basis[ii])))) ii = i;
    if (A[ii][jj] <= EPS) return INF;
    pivot(m, n, ii, jj);
  }
  fill(X, X+n, 0);
  for (i = 1; i <= m; i++) if (basis[i] >= 0) X[basis[i]] = A[i][n];
  return A[0][n];
}


void solve()
{
  int n, m;
  cin >> n >> m;

  string name[100];
  bool used[100] = {false};
  Point loc[100];
  
  for (int i = 0; i < n; i++) {
    cin >> name[i] >> loc[i].x >> loc[i].y;
  }

  double dist[100][100];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dist[i][j] = len(loc[i] - loc[j]);
    }
  }

  double C[MAXM][MAXN];
  for (int i = 0; i <= m+1; i++) {
    for (int j = 0; j <= n; j++) {
      C[i][j] = 0;
    }
  }
  
  for (int j = 0; j < n; j++) {    // obj function
    C[0][j] = -1;
  }

  for (int k = 1; k <= m; k++) {
    string S1, S2, S3, junk;
    cin >> S1 >> junk >> S2 >> junk >> junk >> S3;
    int p1 = find(name, name+n, S1) - name;
    int p2 = find(name, name+n, S2) - name;
    int p3 = find(name, name+n, S3) - name;

    used[p2] = used[p3] = true;
    
    // var2 + dist[p1][p2] <= var3 + dist[p1][p3]
    C[k][p2] = 1;
    C[k][p3] = -1;
    C[k][n] = dist[p1][p3] - dist[p1][p2] - 0.9e-7;
  }

  double X[100];
  double val = simplex(m, n, C, X);
  if (val == -INF) {
    cout << "IMPOSSIBLE" << endl;
    return;
  } 

  pair<double, string> ans[100];
  int ans_n = 0;
  for (int i = 0; i < n; i++) {
    if (used[i]) {
      ans[ans_n++] = make_pair(X[i], name[i]);
    }
  }
  sort(ans, ans+ans_n);
  for (int i = 0; i < ans_n-1; i++) {
    int p1 = find(name, name+n, ans[i].second) - name;
    int p2 = find(name, name+n, ans[i+1].second) - name;

    // var2 <= var1
    C[m+1][p2] = 1;
    C[m+1][p1] = -1;
    C[m+1][n] = -0.9e-7;
    if (simplex(m+1, n, C, X) != -INF) {
      cout << "UNKNOWN" << endl;
      return;
    }
    
    C[m+1][p2] = 0;
    C[m+1][p1] = 0;
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
    
