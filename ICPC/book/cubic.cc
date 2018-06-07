// Gives the solutions a*x^3 + b*x^2 + c*x + d = 0.
//  Note: Does NOT work well when a is NEAR 0.

typedef long double ld;
int cubic(ld a,ld b,ld c,ld d,ld s[3]){
  b /= a, c /= a, d /= a; // Ensure that a is not zero before hand!
  ld q = (b*b - 3*c)/9, r = (2*pow(b,3) - 9*b*c + 27*d) / 54;
  ld z = pow(r,2) - pow(q,3);
  if(z <= EPS){
    ld theta = acos(r/pow(q,1.5));
    for(int i=0;i<3;i++) s[i] = -2*sqrt(q)*cos((theta+i*2*PI)/3) - b/3;
    return 3;
  }
  s[0] = pow(sqrt(z)+fabs(r),1.0/3);
  s[0] = (s[0] + q/s[0]) * (r < 0 ? 1 : -1) - b/3;
  return 1;
}
