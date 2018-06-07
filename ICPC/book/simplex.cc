// simplex: A is (m+1)x(n+1).
// First row obj. function (maximize), next m rows are <= constraints
const int MAX_M = 101, MAX_N = 101; // MAX_CONSTRAINTS+1 and MAX_VARS+1
const double EPS = 1e-9, INF = 1.0/0.0;

void pivot(double A[MAX_M][MAX_N],int m, int n, int a, int b,int basis[],int out[]){
  for (int i = 0; i <= m; i++)
    if (i != a)
      for (int j = 0; j <= n; j++)
	if (j != b) A[i][j] -= A[a][j] * A[i][b] / A[a][b];
  for (int j = 0; j <= n; j++) if (j != b) A[a][j] /=  A[a][b];
  for (int i = 0; i <= m; i++) if (i != a) A[i][b] /= -A[a][b];
  A[a][b] = 1 / A[a][b];
  swap(basis[a], out[b]);
}

bool pless(double a1,double a2,double b1,double b2){
  return (a1 < b1-EPS || (a1 < b1+EPS && a2 < b2));
}

// A is altered
double simplex(int m, int n, double A[MAX_M][MAX_N], double X[MAX_N]){
  int i, j, I, J, basis[MAX_M], out[MAX_N];
  for (i = 1; i <= m; i++) basis[i] = -i;
  for (j = 0; j <= n; j++) A[0][j] = -A[0][j], out[j] = j;
  A[0][n] = 0;
  while(true) {
    for (i = I = 1; i <= m; i++)
      if (make_pair(A[i][n],basis[i]) < make_pair(A[I][n],basis[I])) I = i;
    if (A[I][n] > -EPS) break;
    for (j = J = 0; j < n; j++)
      if (pless(A[I][j],out[J],A[I][J],out[j])) J = j;
    if (A[I][J] > -EPS) return -INF; // No solution
    pivot(A, m, n, I, J, basis, out);
  }
  while(true) {
    for (j = J = 0; j < n; j++)
      if (make_pair(A[0][j],out[j]) < make_pair(A[0][J],out[J])) J = j;
    if (A[0][J] > -EPS) break;
    for (i=1, I=0; i <= m; i++){
      if (A[i][J] < EPS) continue;
      if (!I || pless(A[i][n]/A[i][J],basis[i],A[I][n]/A[I][J],basis[I])) I = i;
    }
    if (A[I][J] < EPS) return INF; // Unbounded
    pivot(A, m, n, I, J, basis, out);
  }
  fill(X, X+n, 0);
  for (i = 1; i <= m; i++) if (basis[i] >= 0) X[basis[i]] = A[i][n];
  return A[0][n];
}
