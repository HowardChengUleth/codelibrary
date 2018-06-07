// -- n is the number of intervals -- IT MUST BE EVEN. O(n)
// -- If K is an upper bound on the 4th derivative of f for all x in [a,b],
//     then the maximum error is ( K*(b-a)^5 ) / ( 180*n^4 )
double integrate(double (*f)(double), double a, double b, int n){
  double ans = f(a) + f(b), h = (b-a)/n;
  for(int i=1;i<n;i++) ans += f(a+i*h) * (i%2 ? 4 : 2);
  return ans * h / 3;
}

// -- h is the step size. Error is O(h^4).
double differentiate(double (*f)(double), double x, double h){
  return (-f(x+2*h) + 8*(f(x+h) - f(x-h)) + f(x-2*h)) / (12*h);
}
