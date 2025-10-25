// Solves Ax = b in floating-point
// - first call LU_decomp on A (returns determinant)
// - then use LU_solve on A, pivot, b to find solution.

double LU_decomp(double A[MAX_N][MAX_N], int n, int pivot[MAX_N]) {
  double s[MAX_N], c, t, det = 1.0;
  
  for (int i = 0; i < n; i++) {
    s[i] = 0.0;
    for (int j = 0; j < n; j++) s[i] = max(s[i], fabs(A[i][j]));
    if (s[i] < EPS) return 0; // Singular
  }
  
  for (int k = 0; k < n; k++){    
    c = fabs(A[k][k]/s[k]), pivot[k] = k;
    for (int i = k+1; i < n; i++)
      if ((t = fabs(A[i][k]/s[i])) > c) { c = t; pivot[k] = i; }
    if (c < EPS) return 0; // Singular
    
    if (k != pivot[k]) {
      det *= -1.0;
      swap_ranges(A[k]+k,A[k]+n,A[pivot[k]]+k);
      swap(s[k],s[pivot[k]]);
    }
    
    for (int i = k+1; i < n; i++) {
      A[i][k] /= A[k][k];
      for (int j = k+1; j < n; j++) A[i][j] -= A[i][k] * A[k][j];
    }
    det *= A[k][k];
  }
  return det;
}

void LU_solve(double A[MAX_N][MAX_N], int n, int pivot[], double b[], double x[]) {
  copy(b, b+n, x);
  for (int k = 0; k < n-1; k++) {
    if (k != pivot[k]) swap(x[k], x[pivot[k]]);
    for (int i = k+1; i < n; i++) x[i] -= A[i][k] * x[k];
  }

  for (int i = n-1; i >= 0; i--) {
    for (int j = i+1; j< n; j++) x[i] -= A[i][j] * x[j];
    x[i] /= A[i][i];
  }
}
