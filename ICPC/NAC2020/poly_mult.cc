// Multiplies two polynomials in O((n+m)*log(n+m))
//  There will be rounding errors. Check for them.

typedef vector<complex<double> > vcd;
vcd DFT(const vcd& a,double inv,int st=0,int step=1){
  int n = a.size()/step;
  if(n == 1) return vcd(1,a[st]);
  complex<double> w_n = polar(1.0,inv*2*PI/n), w = 1;
  vcd y_0 = DFT(a,inv,st,2*step), y_1 = DFT(a,inv,st+step,2*step), c(n);
  
  for(int k=0 ; k<n/2 ; k++,w *= w_n){
    c[k]     = y_0[k] + w*y_1[k];     c[k+n/2] = y_0[k] - w*y_1[k];
  }
  return c;
}

vcd poly_mult(vcd p,vcd q){
  int m = p.size()+q.size(),s=1;
  while(s < m) s *= 2;
  p.resize(s,0); q.resize(s,0);
  vcd P = DFT(p,1), Q = DFT(q,1), R = P;
  for(int i=0;i<R.size();i++) R[i] *= Q[i];
  vcd ans = DFT(R,-1);
  for(int i=0;i<ans.size();i++) ans[i] /= s;
  return ans;
}
