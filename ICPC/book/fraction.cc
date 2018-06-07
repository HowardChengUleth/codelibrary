// Note: Applying operations to reduced fractions should yield a reduced answer
//       Make sure you reduce the fraction when you store it into the structure.
//  EXCEPT: 0 may be 0/x until reduce is called (then changed to 0/1)
typedef long long ll;
struct frac{  ll num,den; };

frac make_frac(ll n,ll d){ frac f; f.num = n,f.den = d; return f; }

frac reduce(frac a){
  if(a.num == 0) return make_frac(0,1); if(a.den < 0) { a.num *= -1; a.den *= -1; }
  ll g = gcd(a.num,a.den); return make_frac(a.num/g,a.den/g);
}
frac recip(frac a){ return reduce(make_frac(a.den,a.num)); }

frac operator+(frac a,frac b){
  ll g = gcd(a.den,b.den);
  return reduce(make_frac(a.num*(b.den/g) + b.num*(a.den/g), (a.den/g)*b.den));
}
frac operator-(frac a,frac b){ return a + make_frac(-b.num,b.den); }
frac operator*(frac a,frac b){
  ll g1 = gcd(a.num,b.den), g2 = gcd(a.den,b.num);
  return make_frac((a.num / g1) * (b.num / g2),(a.den / g2) * (b.den / g1));
}
frac operator/(frac a,frac b){ return a * recip(b); } // Watch division by 0

bool operator==(frac a,frac b){
  a=reduce(a); b=reduce(b);
  return a.num==b.num && a.den==b.den;
}

// Choose one. First one may overflow. Second one has rounding errors.
bool operator<(frac a,frac b){ return (a.num*b.den) < (b.num*a.den); }
bool operator<(frac a,frac b){ return !(a==b) && a.num/1.0/a.den < b.num/1.0/b.den);}
