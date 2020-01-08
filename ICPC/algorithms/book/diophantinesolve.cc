// System of linear diophantine equations   A*x = b
// Returns dim(null space), or -1 if there is no solution.
// xp: a particular solution
// hom_basis: an n x n matrix whose first dim columns form a basis of the nullspace.
// All solutions are obtained by adding integer multiples the basis elements to xp.

#define MAXN 50
#define MAXM 50
int triangulate(int A[MAXN+1][MAXM+MAXN+1], int m, int n, int cols) {
  div_t d;
  int ri = 0, ci = 0;
  while (ri < m && ci < cols) {
    int pi = -1;
    for (int i = ri; i < m; i++) if (A[i][ci] && (pi == -1 || abs(A[i][ci]) < abs(A[pi][ci]))) pi = i;
    if (pi == -1) ci++;
    else {
      int k = 0;
      for (int i = ri; i < m; i++) {
        if (i != pi) {
          d = div(A[i][ci], A[pi][ci]);
          if (d.quot) {
            for (int j = ci; j < n; j++) A[i][j] -= d.quot*A[pi][j]; 
            k++;
          } 
        }
      }
      if (!k) {
        for (int i = ci; i < n && ri != pi; i++) swap(A[ri][i], A[pi][i]);
        ri++;	ci++; 
      } 
    } 
  }
  return ri;
}

int diophantine_linsolve(int A[MAXM][MAXN], int b[MAXM], int m, int n, int xp[MAXN], int hom_basis[MAXN][MAXN]) {
  int mat[MAXN+1][MAXM+MAXN+1], i, j, rank, d;
  for (i = 0; i < m; i++) mat[0][i] = -b[i];
  for (i = 0; i < m; i++) for (j = 0; j < n; j++) mat[j+1][i] = A[i][j];
  for (i = 0; i < n+1; i++) for (j = 0; j < n+1; j++) mat[i][j+m] = (i == j);
  rank = triangulate(mat, n+1, m+n+1, m+1);
  d = mat[rank-1][m];
  if (d != 1 && d != -1) return -1; // no integer solutions
  for (i = 0; i < m; i++)
    if (mat[rank-1][i]) return -1; // inconsistent system
  for (i = 0; i < n; i++) {
    xp[i] = d*mat[rank-1][m+1+i];
    for (j = 0; j < n+1-rank; j++) hom_basis[i][j] = mat[rank+j][m+1+i];
  }
  return n+1-rank;
}
