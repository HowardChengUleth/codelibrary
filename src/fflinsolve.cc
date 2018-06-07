/*
 * Solution of systems of linear equations over the integers
 *
 * Author: Howard Cheng
 * Reference:
 *  K.O. Geddes, S.R. Czapor, G. Labahn.  "Algorithms for Computer Algebra."
 *    Kluwer Academic Publishers, 1992, pages 393-399.  ISBN 0-7923-9259-0
 *
 * The routine fflinsolve solves the system Ax = b where A is an n x n matrix
 * of integers and b is an n-dimensional vector of integers.
 *
 * The inputs to fflinsolve are the matrix A, the dimension n, and an
 * output array to store the solution x_star = det(A)*x.  The function
 * also returns the det(A).  In the case that det(A) = 0, the solution
 * vector is undefined.
 *
 * Note that the matrix A and b may be modified.
 *
 */

#include <iostream>

using namespace std;

const int MAX_N = 10;

int fflinsolve(int A[MAX_N][MAX_N], int b[], int x_star[], int n)
{
  int sign, d, i, j, k, k_c, k_r, pivot, t;

  sign = d = 1;

  for (k_c = k_r = 0; k_c < n; k_c++) {
    /* eliminate column k_c */
    
    /* find nonzero pivot */
    for (pivot = k_r; pivot < n && !A[pivot][k_r]; pivot++)
      ;
    
    if (pivot < n) {
      /* swap rows pivot and k_r */
      if (pivot != k_r) {
	for (j = k_c; j < n; j++) {
	  t = A[pivot][j];
	  A[pivot][j] = A[k_r][j];
	  A[k_r][j] = t;
	}
	t = b[pivot];
	b[pivot] = b[k_r];
	b[k_r] = t;

	sign *= -1;
      }

      /* do elimination */
      for (i = k_r+1; i < n; i++) {
	for (j = k_c+1; j < n; j++) {
	  A[i][j] = (A[k_r][k_c]*A[i][j]-A[i][k_c]*A[k_r][j])/d;
	}
	b[i] = (A[k_r][k_c]*b[i]-A[i][k_c]*b[k_r])/d;
	A[i][k_c] = 0;
      }
      if (d) {
	d = A[k_r][k_c];
      }
      k_r++;
    } else {
      /* entire column is 0, det(A) = 0 */
      d = 0;
    }
  }

  if (!d) {
    for (k = k_r; k < n; k++) {
      if (b[k]) {
	/* inconsistent system */
	cout << "Inconsistent system." << endl;
	return 0;
      }
    }
    /* multiple solutions */
    cout << "More than one solution." << endl;
    return 0;
  }

  /* now backsolve */
  for (k = n-1; k >= 0; k--) {
    x_star[k] = sign*d*b[k];
    for (j = k+1; j < n; j++) {
      x_star[k] -= A[k][j]*x_star[j];
    }
    x_star[k] /= A[k][k];
  }

  return sign*d;
}

int main(void)
{
  int A[MAX_N][MAX_N], x_star[MAX_N], b[MAX_N];
  int n, i, j;
  int det;

  while (cin >> n && 0 < n && n <= MAX_N) {
    cout << "Enter A:" << endl;
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
	cin >> A[i][j];
      }
    }
    cout << "Enter b:" << endl;
    for (i = 0; i < n; i++) {
      cin >> b[i];
    }
    if (det = fflinsolve(A, b, x_star, n)) {
      cout << "det = " << det << endl;
      cout << "x_star = ";
      for (i = 0; i < n; i++) {
	cout << x_star[i] << " ";
      }
      cout << endl;
    } else {
      cout << "A is singular." << endl;
    }
  }
  return 0;
}
