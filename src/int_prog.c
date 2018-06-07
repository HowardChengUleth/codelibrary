/*
 * All-integer programming
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.cs.sunysb.edu/~algorith/implement/syslo/distrib/processed/
 *
 * This algorithm is based on GOMORY cutting plane method.
 *                  
 * This algorithm solves the following INTEGER LP problem:
 *
 * minimize       SUM  (A[0][j] * x[j])         [cost function]
 *            (j=0 to n-1) 
 *
 *     s.t.       SUM   (A[i][j]*x[j])  <=  A[i][n]    1 <= i <= m
 *            (j=0 to n-1)
 *
 *     and        x[j] >= 0         0 <= j <= n-1
 *
 * n = number of variables
 * m = number of constraints
 *
 * Input : An input array A with m+n+1 rows and n+1 columns.
 *         Store the cost function in row 0, and the constraints in rows
 *           1 to m.  Set A[0][n] = 0.
 *         A vector x allocated for n values to store returned value.
 *
 * Output: Returns 1 if a solution is found, 0 if no solution exists.
 *         The minimum value of the cost function is returned in 
 *           value.
 *         The variable assignment to x[j] that gives the minimum is given 
 *           in x[j], where 0 <= j <= n-1.
 *
 * Important Notes:
 *
 * 1. If we want to have constraints that are >=, just multiply all the
 *    coefficients by -1.
 * 2. If we want to have constraints that are ==, do both >= and <=.
 * 3. The contents of A is destroyed after this routine.
 * 4. The coefficients in the cost function must be positive.  If not,
 *    make a change of variable x'[i] = m[i]-x[i] where m[i] is the
 *    maximum value for variable[i] and adjust all constraints as well
 *    as the returned optimal value.  This is especially useful if you
 *    wish to maximize the cost function.
 *
 *    Usually there is some maximum for each variable if you wish to 
 *    maximize the function (or the value could be infinity.
 *
 *    NOTE: if any coefficient in the objective function is negative or
 *          0, the routine will crash.
 *
 * 5. If one only wishes to know if there is any variable assignment
 *    satisfying the constraints, just put 1 in each coefficient
 *    of the objective function.
 */

#include <stdio.h>
#include <assert.h>


#define MAX_VARS  50
#define MAX_CONS  50
#define MAX_ROWS  MAX_VARS+MAX_CONS+1
#define MAX_COLS  MAX_VARS+1

int euclid(int u, int v)
{
  int w = u / v;
  if (w*v > u) {
    w--;
  }
  if ((w+1)*v <= u) {
    w++;
  }
  return w;
}

int int_prog(int A[MAX_ROWS][MAX_COLS], int n, int m, int *value, int *x)
{
  int iter, nosol;
  int b, c, i, j, k, l, r, r1, s, t, denom, num;

  for (j = 0; j < n; j++) {
    if (A[0][j] <= 0) {
      // BAD objective function coefficient: make sure it is positive
      assert(false);
    }
  }
  
  /* set constraints that x[j] >= 0, and clear output */
  for (i = 0; i < n; i++) {
    for (j = 0; j < n+1; j++) {
      A[m+1+i][j] = 0;
    }
    A[m+1+i][i] = -1;
  }
  A[0][n] = 0;

  nosol = 0;
  do {
    r = 0;
    do {
      iter = (A[++r][n] < 0);
    } while (!iter && r != n+m);
    if (iter) {
      for (k = iter = 0; k < n && !iter; k++) {
	iter = (A[r][k] < 0);
      }
      nosol = !iter;
      if (iter) {
	l = k-1;
	for (j = k; j < n; j++) {
	  if (A[r][j] < 0) {
	    for (i = 0; !(s = A[i][j] - A[i][l]); i++)
	      ;
	    if (s < 0) {
	      l = j;
	    }
	  }
	}
	for (s = 0; !A[s][l]; s++)
	  ;
	num = -A[r][l];
	denom = 1;
	for (j = 0; j < n; j++) {
	  if (A[r][j] < 0 && j != l) {
	    for (i = s-1, b = 1; b && i >= 0; i--) {
	      b = (A[i][j] == 0);
	    }
	    if (b) {
	      i = A[s][j];
	      r1 = A[s][l];
	      t = euclid(i, r1);
	      if (t*r1 == i && t > 1) {
		for (i = s+1; !(r1 = t*A[i][l] - A[i][j]); i++)
		  ;
		if (r1 > 0) {
		  t--;
		}
	      }
	      c = -A[r][j];
	      if (c*denom > t*num) {
		num = c;
		denom = t;
	      }
	    }
	  }
	}
	for (j = 0; j <= n; j++) {
	  if (j != l) {
	    c = euclid(A[r][j]*denom, num);
	    if (c) {
	      for (i = 0; i <= n+m; i++) {
		A[i][j] += c*A[i][l];
	      }
	    }
	  }
	}
      }
    }
  } while (iter && !nosol);

  *value = -A[0][n];
  for (j = 0; j < n; j++) {
    x[j] = A[m+1+j][n];
  }

  return !nosol;
}

int main(void)
{
  int A[MAX_ROWS][MAX_COLS];
  int x[MAX_VARS];
  int val, t;
  int m, n, i, j;

  while (scanf("%d %d", &n, &m) == 2 && n > 0 && m > 0) {
    /* read cost function */
    printf("Input cost function to minimize:\n");
    for (i = 0; i < n; i++) {
      scanf("%d", &A[0][i]);
    }

    /* read constraints */
    for (i = 1; i <= m; i++) {
      printf("Input constraint #%d:\n", i);
      for (j = 0; j < n+1; j++) {
	scanf("%d", &A[i][j]);
      }
    }

    t = int_prog(A, n, m, &val, x);
    if (t) {
      printf("Minimum cost = %d\n", val);
      for (i = 0; i < n; i++) {
	printf("x[%2d] = %2d\n", i, x[i]);
      } 
    } else {
      printf("No solution exists.\n");
    }
  }

  return 0;
}
