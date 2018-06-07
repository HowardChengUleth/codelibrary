/*
 * Solution of system of linear diophantine equations
 *
 * Author: Howard Cheng
 * Date:   Nov 25, 2000
 * Reference:
 *
 * http://scicomp.ewha.ac.kr/netlib/tomspdf/
 *
 * Look at Algorithms 287 (sort of) and 288.
 *
 * Given a system of m linear diophantine equations in n unknowns,
 * this algorithm finds a particular solution as well as a basis for
 * the solution space of the homogeneous system, if they exist.  The
 * system is represented in matrix form as Ax = b where all entries
 * are integers.
 *
 * Function: diophantine_linsolve
 *
 * Input:
 *
 * A: an m x n matrix specifying the coefficients of each equation in
 *    each row (it is okay to have zero rows, or even have A = 0)
 * b: an m-dimensional vector specifying the right-hand side of the system
 * m: number of equations in the system
 * n: number of unknowns in the system
 *
 * Output:
 *
 * The function returns the dimension of the solution space of the
 * homogeneous system Ax = 0 (hom_dim) if it has a solution.
 * Otherwise, it returns -1.
 *
 * Other results returned in the parameters are:
 *
 * xp: an n-dimensional vector giving a particular solution
 * hom_basis: an n x n matrix whose first hom_dim columns form a basis
 *            of the solution space of the homogeneous system Ax = 0
 *
 * All solutions to Ax = b can be obtained by adding integer multiples
 * of the first hom_dim columns of hom_basis to xp.
 *
 * Note:
 *
 * The contents of A and b are not changed by this function.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 50
#define MAX_M 50

int triangulate(int A[MAX_N+1][MAX_M+MAX_N+1], int m, int n, int cols)
{
  int ri, ci, i, j, k, pi, t;
  div_t d;

  ri = ci = 0;
  while (ri < m && ci < cols) {

    /* find smallest non-zero pivot */
    pi = -1;
    for (i = ri; i < m; i++) {
      if (A[i][ci] && (pi == -1 || abs(A[i][ci]) < abs(A[pi][ci]))) {
	pi = i;
      }
    }
    if (pi == -1) {
      /* the entire column is 0, skip it */
      ci++;
    } else {
      k = 0;
      for (i = ri; i < m; i++) {
	if (i != pi) {
	  d = div(A[i][ci], A[pi][ci]);
	  if (d.quot) {
	    k++;
	    for (j = ci; j < n; j++) {
	      A[i][j] -= d.quot*A[pi][j];
	    }
	  }
	}
      }
      if (!k) {
	/* swap the row to make it triangular...Alg 287 also switches the */
	/* sign, probably to preserve the sign of the minors.  I don't    */
	/* think this is necessary for our purpose.                       */
	for (i = ci; i < n && ri != pi; i++) {
	  t = A[ri][i];
	  A[ri][i] = A[pi][i];
	  A[pi][i] = t;
	}
	ri++;
	ci++;
      }
    }
  }
  return ri;
}

int diophantine_linsolve(int A[MAX_M][MAX_N], int b[MAX_M], int m, int n,
			 int xp[MAX_N], int hom_basis[MAX_N][MAX_N])
{
  int mat[MAX_N+1][MAX_M+MAX_N+1];
  int i, j, rank, d;

  /* form the work matrix */
  for (i = 0; i < m; i++) {
    mat[0][i] = -b[i];
  }
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      mat[j+1][i] = A[i][j];
    }
  }
  for (i = 0; i < n+1; i++) {
    for (j = 0; j < n+1; j++) {
      mat[i][j+m] = (i == j);
    }
  }

  /* triangluate the first n+1 x m+1 submatrix */
  rank = triangulate(mat, n+1, m+n+1, m+1);
  d = mat[rank-1][m];

  /* check for no solutions */
  if (d != 1 && d != -1) {
    /* no integer solutions */
    return -1;
  }
  /* check for inconsistent system */
  for (i = 0; i < m; i++) {
    if (mat[rank-1][i]) {
      return -1;
    }
  }

  /* there is a solution, copy it to the result */
  for (i = 0; i < n; i++) {
    xp[i] = d*mat[rank-1][m+1+i];
    for (j = 0; j < n+1-rank; j++) {
      hom_basis[i][j] = mat[rank+j][m+1+i];
    }
  }

  return n+1-rank;
}

int main(void)
{
  int A[MAX_M][MAX_N], b[MAX_M], m, n, xp[MAX_N], hom_basis[MAX_N][MAX_N];
  int i, j, hom_dim;

  while (scanf("%d %d", &m, &n) == 2 && m > 0 && n > 0) {
    for (i = 0; i < m; i++) {
      printf("Enter equation %d:\n", i+1);
      for (j = 0; j < n; j++) {
	scanf("%d", &A[i][j]);
      }
      scanf("%d", &b[i]);
    }

    if ((hom_dim = diophantine_linsolve(A, b, m, n, xp, hom_basis)) >= 0) {
      printf("Particular solution:\n");
      for (i = 0; i < n; i++) {
	printf("%d ", xp[i]);
      }
      printf("\n");
      printf("hom_dim = %d\n", hom_dim);
      printf("Basis for Ax = 0:\n");
      for (j = 0; j < hom_dim; j++) {
	for (i = 0; i < n; i++) {
	  printf("%d ", hom_basis[i][j]);
	}
	printf("\n");
      }
    } else {
      printf("No solution.\n");
    }

  }

  return 0;
}
