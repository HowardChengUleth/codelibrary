#include <algorithm>

using namespace std;

const int MAX_CONSTRAINTS = 100;
const int MAX_VARS = 100;
const int MAXM = MAX_CONSTRAINTS + 1;
const int MAXN = MAX_VARS + 1;

const double EPS = 1e-9;
const double INF = 1.0/0.0;

double A[MAXM][MAXN];
int basis[MAXM], out[MAXN];

void pivot(int m, int n, int a, int b)
{
  int i, j;
  for (i = 0; i <= m; i++)
    if (i != a)
      for (j = 0; j <= n; j++)
	if (j != b)
	  A[i][j] -= A[a][j] * A[i][b] / A[a][b];
  for (j = 0; j <= n; j++)
    if (j != b) A[a][j] /= A[a][b];
  for (i = 0; i <= m; i++)
    if (i != a) A[i][b] = -A[i][b] / A[a][b];
  A[a][b] = 1 / A[a][b];
  swap(basis[a], out[b]);
}

double simplex(int m, int n, double C[][MAXN], double X[])
{
  int i, j, ii, jj;
  for (i = 1; i <= m; i++) 
    copy(C[i], C[i]+n+1, A[i]);
  for (j = 0; j <= n; j++)
    A[0][j] = -C[0][j];
  for (i = 0; i <= m; i++)
    basis[i] = -i;
  for (j = 0; j <= n; j++)
    out[j] = j;
  for (;;) {
    for (i = ii = 1; i <= m; i++)
      if (A[i][n] < A[ii][n] || (A[i][n] == A[ii][n] && basis[i] < basis[ii]))
	ii = i;
    if (A[ii][n] >= -EPS) break;
    for (j = jj = 0; j < n; j++)
      if (A[ii][j] < A[ii][jj] - EPS ||
	  (A[ii][j] < A[ii][jj] - EPS && out[i] < out[j]))
	jj = j;
    if (A[ii][jj] >= -EPS) return -INF;
    pivot(m, n, ii, jj);
  }
  for (;;) {
    for (j = jj = 0; j < n; j++)
      if (A[0][j] < A[0][jj] || (A[0][j] == A[0][jj] && out[j] < out[jj]))
	jj = j;
    if (A[0][jj] > -EPS) break;
    for (i=1, ii=0; i <= m; i++)
      if ((A[i][jj]>EPS) &&
	  (!ii || (A[i][n]/A[i][jj] < A[ii][n]/A[ii][jj]-EPS) ||
	   ((A[i][n]/A[i][jj] < A[ii][n]/A[ii][jj]+EPS) &&
	    (basis[i] < basis[ii]))))
	ii = i;
    if (A[ii][jj] <= EPS) return INF;
    pivot(m, n, ii, jj);
  }
  fill(X, X+n, 0);
  for (i = 1; i <= m; i++)
    if (basis[i] >= 0)
      X[basis[i]] = A[i][n];
  return A[0][n];
}

#include <iostream>
#include <iomanip>


int main(void)
{
  double C[MAXM][MAXN], X[MAX_VARS];

  C[0][0] = -1;   C[0][1] = -3;  C[0][2] = 0;  C[0][3] = 0;
  C[1][0] = -2;  C[1][1] = -3;  C[1][2] = -6; C[1][3] = 250;
  C[2][0] = -1;   C[2][1] = -5;   C[2][2] = -5;  C[2][3] = 400;
  C[3][0] = -59;   C[3][1] = -35;   C[3][2] = -160;  C[3][3] = 30;

  double val = simplex(2, 2, C, X);

  cout << fixed << setprecision(3);
  cout << "val = " << val << endl;
  cout << "X[0] = " << X[0] << endl;
  cout << "X[1] = " << X[1] << endl;
  //  cout << "X[2] = " << X[2] << endl;
  return 0;
}
