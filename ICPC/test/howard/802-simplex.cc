#include <iostream>
#include <algorithm>

using namespace std;

// simplex: C is (m+1)x(n+1).
// First row obj. function (maximize), next m rows are <= constraints
const int MAX_CONSTRAINTS = 6;
const int MAX_VARS = 100;
const int MAXM = MAX_CONSTRAINTS + 1, MAXN = MAX_VARS + 1;
const double EPS = 1e-9, INF = 1.0/0.0;
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


////////////////////////////////////////////////////////////////////////////

bool solve(int id)
{
  int n;
  cin >> n;
  if (!n) return false;

  if (id > 1) cout << endl;
  cout << "Mixture " << id << endl;

  int a[100], b[100], c[100];
  for (int i = 0; i < n; i++) {
    cin >> a[i] >> b[i] >> c[i];
  }

  double C[MAXM][MAXN];
  double X[MAXN];

  int ta, tb, tc;
  cin >> ta >> tb >> tc;

  for (int i = 0; i < n; i++) {
    C[0][i] = 1;
    C[1][i] = a[i];  
    C[2][i] = -a[i]; 
    C[3][i] = b[i];  
    C[4][i] = -b[i]; 
    C[5][i] = c[i];  
    C[6][i] = -c[i]; 
  }

  C[1][n] = ta; 
  C[2][n] = -ta;
  C[3][n] = tb; 
  C[4][n] = -tb;
  C[5][n] = tc; 
  C[6][n] = -tc;

  if (simplex(6, n, C, X) > -INF) {
    cout << "Possible" << endl;
  } else {
    cout << "Impossible" << endl;
  }
  return true;
}

int main()
{
  int id = 1;
  while (solve(id++))
    ;
  return 0;
}
