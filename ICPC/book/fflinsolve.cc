// solves Ax = b.  Returns det...solution is x_star[i]/det
// A and b may be modified!
int fflinsolve(int A[MAX_N][MAX_N], int b[], int x_star[], int n) {
  int k_c, k_r, pivot, sign = 1, d = 1;
  for (k_c = k_r = 0; k_c < n; k_c++) {
    for (pivot = k_r; pivot < n && !A[pivot][k_r]; pivot++) ;
    if (pivot < n) {
      if (pivot != k_r) {
	for (int j = k_c; j < n; j++) swap(A[pivot][j], A[k_r][j]);
	swap(b[pivot], b[k_r]);     sign *= -1;
      }

      for (int i = k_r+1; i < n; i++) {
	for (int j = k_c+1; j < n; j++) 
	  A[i][j] = (A[k_r][k_c]*A[i][j]-A[i][k_c]*A[k_r][j])/d;
	b[i] = (A[k_r][k_c]*b[i]-A[i][k_c]*b[k_r])/d;
        A[i][k_c] = 0;
      }
      if (d) d = A[k_r][k_c];
      k_r++;
    } else d = 0;
  }
  if (!d) {
    for (int k = k_r; k < n; k++) if (b[k]) return 0;   // inconsistent system
    return 0;                                           // multiple solutions
  }
  for (int k = n-1; k >= 0; k--) {
    x_star[k] = sign*d*b[k];
    for (int j = k+1; j < n; j++) x_star[k] -= A[k][j]*x_star[j];
    x_star[k] /= A[k][k];
  }
  return sign*d;
}
