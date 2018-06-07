/*
 * Zero-one programming
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.cs.sunysb.edu/~algorith/implement/syslo/distrib/processed/
 *
 * This algorithm is based on BALAS branching testing.
 *
 * This algorithm solves the BINARY linear program:
 *
 *      min  cx                [cost function]
 *      s.t.
 *           Ax <= b           [constraints]
 *           x[i] = 0 or 1.
 *
 * where A is an m x n matrix, 
 *       c and x are n-dimensional vectors,
 *       b is an m-dimensional vector.
 *
 * n = number of variables
 * m = number of constraints
 *
 * It returns whether there exists a solution.
 * The optimal value of the cost function is returned in value.
 * The assignment giving the optimal cost function value is returned in x.
 *
 * Important Notes:
 *
 * 1. The matrices and arrays start their indices at 1!!!!!!
 * 2. If we want to have constraints that are >=, just multiply all the
 *    coefficients by -1.
 * 3. If we want to have constraints that are ==, do both >= and <=.
 * 4. The content of A, b, and c is preserved after this routine.
 * 5. The coefficients in the cost vector c must be positive.  If not,
 *    make a change of variable x'[i] = 1-x[i] and adjust all constraints
 *    as well as the returned optimal value.  This is especially useful
 *    if you wish to maximize the cost function.
 *
 */

#include <stdio.h>
#include <limits.h>
#include <assert.h>

#define MAX_VAR 1000
#define MAX_CONS 100
#define MAX_ROWS MAX_CONS+1
#define MAX_COLS MAX_VAR+1

int zero_one(int A[MAX_ROWS][MAX_COLS], int *b, int *c, int n, int m, 
	     int *val, int *x)
{
  int exist;
  int alpha, beta, gamma, i, j, mnr, nr;
  int p, r, r1, r2, s, t, z;
  int y[MAX_ROWS], w[MAX_ROWS], zr[MAX_ROWS];
  int ii[MAX_COLS], jj[MAX_COLS], xx[MAX_COLS];
  int kk[MAX_COLS+1];

  for (i = 1; i <= m; i++) {
    y[i] = b[i];
  }
  z = 1;
  for (j = 1; j <= n; j++) {
    xx[j] = 0;
    z += c[j];
  }
  *val = z+z;
  s = t = z = exist = 0;
  kk[1] = 0;
m10:
  p = mnr = 0;
  for (i = 1; i <= m; i++) {
    if ((r = y[i]) < 0) {
      p++;
      gamma = 0;
      alpha = r;
      beta = -INT_MAX;
      for (j = 1; j <= n; j++) {
	if (xx[j] <= 0) {
	  if (c[j] + z >= *val) {
	    xx[j] = 2;
	    kk[s+1]++;
	    jj[++t] = j;
	  } else {
	    if ((r1 = A[i][j]) < 0) {
	      alpha -= r1;
	      gamma += c[j];
	      if (beta < r1) {
		beta = r1;
	      }
	    }
	  }
	}
      }
      if (alpha < 0) {
	goto m20;
      }
      if (alpha + beta < 0) {
	if (gamma + z >= *val) {
	  goto m20;
	}
	for (j = 1; j <= n; j++) {
	  r1 = A[i][j];
	  r2 = xx[j];
	  if (r1 < 0) {
	    if (!r2) {
	      xx[j] = -2;
	      for (nr = 1; nr <= mnr; nr++) {
		zr[nr] -= A[w[nr]][j];
		if (zr[nr] < 0) {
		  goto m20;
		}
	      }
	    }
	  } else {
	    if (r2 < 0) {
	      alpha -= r1;
	      if (alpha < 0) {
		goto m20;
	      }
	      gamma += c[j];
	      if (gamma + z >= *val) {
		goto m20;
	      }
	    }
	  }
	}
	mnr++;
	w[mnr] = i;
	zr[mnr] = alpha;
      }
    }
  }

  if (!p) {
    *val = z;
    exist = 1;
    for (j = 1; j <= n; j++) {
      x[j] = (xx[j] == 1) ? 1 : 0;
    }
    goto m20;
  }

  if (!mnr) {
    p = 0;
    gamma = -INT_MAX;
    for (j = 1; j <= n; j++) {
      if (!xx[j]) {
	beta = 0;
	for (i = 1; i <= m; i++) {
	  r = y[i];
	  r1 = A[i][j];
	  if (r < r1) {
	    beta += r - r1;
	  }
	}
	r = c[j];
	if ((beta > gamma) ||
	    (beta == gamma && r < alpha)) {
	  alpha = r;
	  gamma = beta;
	  p = j;
	}
      }
    }
    if (!p) {
      goto m20;
    }
    s++;
    kk[s+1] = 0;
    jj[++t] = p;
    ii[s] = xx[p] = 1;
    z += c[p];
    for (i = 1; i <= m; i++) {
      y[i] -= A[i][p];
    }
  } else {
    s++;
    ii[s] = kk[s+1] = 0;
    for (j = 1; j <= n; j++) {
      if (xx[j] < 0) {
	jj[++t] = j;
	ii[s]--;
	z += c[j];
	xx[j] = 1;
	for (i = 1; i <= m; i++) {
	  y[i] -= A[i][j];
	}
      }
    }
  }
  goto m10;

m20:
  for (j = 1; j <= n; j++) {
    if (xx[j] < 0) {
      xx[j] = 0;
    }
  }
  if (s > 0) {
    do {
      p = t;
      t -= kk[s+1];
      for (j = t+1; j <= p; j++) {
	xx[jj[j]] = 0;
      }
      p = (ii[s] >= 0) ? ii[s] : -ii[s];
      kk[s] += p;
      for (j = t-p+1; j <= t; j++) {
	p = jj[j];
	xx[p] = 2;
	z -= c[p];
	for (i = 1; i <= m; i++) {
	  y[i] += A[i][p];
	}
      }
      s--;
      if (ii[s+1] >= 0) {
	goto m10;
      }
    } while (s);
  }

  return exist;
}

int main(void)
{
  int A[MAX_ROWS][MAX_COLS];
  int c[MAX_COLS], x[MAX_COLS], b[MAX_ROWS];
  int val, t;
  int m, n, i, j;

  while (scanf("%d %d", &n, &m) == 2 && n > 0 && m > 0) {
    /* read cost function */
    printf("Input cost function to minimize:\n");
    for (i = 1; i <= n; i++) {
      scanf("%d", &c[i]);
    }

    /* read constraints */
    for (i = 1; i <= m; i++) {
      printf("Input constraint #%d:\n", i);
      for (j = 1; j <= n; j++) {
	scanf("%d", &A[i][j]);
      }
      scanf("%d", &b[i]);
    }

    t = zero_one(A, b, c, n, m, &val, x);
    if (t) {
      printf("Minimum cost = %d\n", val);
      for (i = 1; i <= n; i++) {
	printf("x[%2d] = %2d\n", i, x[i]);
      } 
    } else {
      printf("No solution exists.\n");
    }
  }

  return 0;
}
