// Big integer implementation
using namespace std::rel_ops;

typedef long long Digit;
#define BASE 1000000000
#define LOG_BASE 9

#define pbb pair<BigInteger,BigInteger>
#define VEC(v,i) ((0 <= i && i < v.mag.size()) ? v.mag[i] : 0)

bool isZ(Digit x){ return x; }
struct BigInteger {
  BigInteger(Digit n = 0);
  BigInteger(string s);    // no error checking
  int sign;           // +1 = positive, 0 = zero, -1 = negative
  vector<Digit> mag;  // magnitude
  void clear() { sign = 0;   mag.clear(); }
  void normalize(){
    mag.resize(mag.rend()-find_if(mag.rbegin(),mag.rend(),isZ));
    if(mag.empty()) clear(); }
  
  string toString() const;        // convert to string
  long long toLongLong() const { return strtoll(toString().c_str(),NULL,10); }
  
  bool isZero() const { return sign == 0; }
  bool operator<(const BigInteger &a) const;
  bool operator==(const BigInteger &a) const { return sign==a.sign && mag==a.mag; }    

  BigInteger operator-() const { BigInteger t(*this); t.sign *= -1; return t; }
  friend BigInteger add_sub(BigInteger a,const BigInteger& b,int m);
  friend pbb divide(const BigInteger &a,const BigInteger &b);
  BigInteger &operator+=(const BigInteger &a){ return *this = add_sub(*this,a,1); }
  BigInteger &operator-=(const BigInteger &a){ return *this = add_sub(*this,a,-1); }
  BigInteger &operator*=(const BigInteger &a);
  BigInteger &operator/=(const BigInteger &a){ return *this = divide(*this,a).first; }
  BigInteger &operator%=(const BigInteger &a){ return *this = divide(*this,a).second;}
  // This is (*= BASE^a), not (*= 2^a)
  BigInteger &operator<<=(Digit a){ if(sign)mag.insert(mag.begin(),a,0);return *this;}
  bool sqrt(BigInteger &root) const;
};

BigInteger operator+(BigInteger a, const BigInteger &b) { return a += b; }
BigInteger operator-(BigInteger a, const BigInteger &b) { return a -= b; }
BigInteger operator*(BigInteger a, const BigInteger &b) { return a *= b; }
BigInteger operator/(BigInteger a, const BigInteger &b) { return a /= b; }
BigInteger operator%(BigInteger a, const BigInteger &b) { return a %= b; }
ostream &operator<<(ostream &os, const BigInteger &a){ return (os << a.toString()); }

BigInteger::BigInteger(Digit n){
  if (n == 0){ clear(); return; } sign = (n<0 ? -1 : 1); n *= sign;  
  while (n > 0) { mag.push_back(n % BASE); n /= BASE; }
}

BigInteger::BigInteger(string s){
  clear(); sign = 1; if(s[0] == '-'){ sign = -1; s[0] = '0'; }
  int n = s.length(); mag.resize((n+LOG_BASE-1)/LOG_BASE,0); vector<Digit>::reverse_iterator p = mag.rbegin();
  for(int i=0;i<n;i++){
    if(i && ((n-i) % 9 == 0)) p++;
    (*p) = (*p)*10 + (s[i] - '0');
  } normalize();
}

string BigInteger::toString() const {
  if(sign == 0) return "0";
  stringstream ss; if(sign == -1) ss << "-"; ss << *mag.rbegin();
  for(int i=mag.size()-2;i>=0;i--) ss << setw(LOG_BASE) << setfill('0') << mag[i];
  return ss.str();
}

bool BigInteger::operator<(const BigInteger &a) const {
  if (sign != a.sign) return sign < a.sign;
  if (sign < 0) return -a < -(*this);
  if (mag.size() != a.mag.size()) return mag.size() < a.mag.size();
  return lexicographical_compare(mag.rbegin(),mag.rend(),a.mag.rbegin(),a.mag.rend());
}

BigInteger add_sub(BigInteger a,const BigInteger& b,int m){
  if(b.sign == 0) return a; if(a.sign == 0) return (m == 1 ? b : -b);
  if(a.sign != b.sign) return add_sub(a,-b,-m);
  if(a.sign == -1) return -add_sub(-a,-b,m);
  if(a < b){ BigInteger x = add_sub(b,a,m); return (m == 1 ? x : -x); }
  Digit bc = 0,lim = a.mag.size();
  for(Digit i=0;i<lim;i++){
    Digit ds = VEC(a,i) + m*VEC(b,i) + m*bc;
    if((m>0 && ds>=BASE) || (m<0 && ds<0)){ a.mag[i] = ds - m*BASE; bc = 1; }
    else{ a.mag[i] = ds; bc = 0; }
  }
  if(bc) a.mag.push_back(1); a.normalize(); return a;
}

BigInteger int_mult(BigInteger b,Digit a){
  if (b.sign == 0 || a == 0){ b.clear(); return b; }
  if (a < 0){ b.sign *= -1; a = -a; }
  Digit carry = 0, n = b.mag.size();
  for (int i = 0; i < n; i++) {
    Digit x = a * b.mag[i] + carry; b.mag[i] = x % BASE; carry = x / BASE; }
  if (carry) b.mag.push_back(carry); return b;
}

BigInteger &BigInteger::operator*=(const BigInteger &a){
  BigInteger t(*this),c;
  if (this == &a) c = a; const BigInteger &b = (this == &a) ? c : a; clear();
  for (int i=0;i<b.mag.size();i++){ *this += int_mult(t,b.mag[i]); t <<= 1; }
  sign *= b.sign; return (*this);
}

pbb divide(const BigInteger &a,const BigInteger &b){
  if(a.sign*b.sign == 0) return make_pair(0,0); // WARNING: x/0 == 0, x%0 == 0
  if(b.sign == -1){ pbb t=divide( a,-b); t.first=-t.first; return t; }
  if(a.sign == -1){ pbb t=divide(-a, b); BigInteger q(t.first),r(t.second);
    q=-q; r=-r; if(r < 0){ r+=b; q-=1; } return make_pair(q,r); }
  if(a < b) return make_pair(0,a);
  BigInteger q,r; q.sign = 1;
  
  if (b.mag.size() == 1){
    Digit R = 0;
    for (int i=a.mag.size()-1;i>=0;i--){
      Digit t = R * BASE + a.mag[i];
      q.mag.insert(q.mag.begin(), t / b.mag[0]);
      R = t - q.mag[0] * b.mag[0];
    }
    q.normalize(); r = R;
    return make_pair(q,r);
  }

  Digit t,q2,r2,n,m,d;
  r = a;
  n = b.mag.size(), m = a.mag.size() - n, d = BASE / (*b.mag.rbegin() + 1);
  q.mag.resize(m+1); q.sign = 1; r.mag.resize(m+n+1,0); BigInteger v(b);
  r *= d;    v *= d;
  
  for (int j = m; j >= 0; j--) {
    t=r.mag[j+n]*BASE+r.mag[j+n-1]; q2=t/v.mag[n-1]; r2=t-q2*v.mag[n-1];
    
#define XXX (q2 == BASE || q2 * v.mag[n-2] > BASE * r2 + r.mag[j+n-2])
    if (XXX){ q2--; r2 += v.mag[n-1];
      if (r2 < BASE && XXX){ q2--; r2 += v.mag[n-1]; } }
    
    Digit carry = 0, borrow = 0;
    for (int i = 0; i <= n; i++) {
      t = q2 * VEC(v,i) + carry; carry = t / BASE; t %= BASE;
      Digit diff = r.mag[j+i] - t - borrow;
      r.mag[j+i] = diff + BASE*(diff < 0 && i < n); borrow = (diff < 0);
    }
    
    q.mag[j] = q2;
    if (r.mag[n+j] < 0) {
      q.mag[j]--;
      carry = 0;
      for (int i = 0; i < n; i++) {
	t = r.mag[j+i] + v.mag[i] + carry;
	r.mag[j+i] = t % BASE; carry = t / BASE;
      }
      r.mag[j+n] += carry;
    }
  }
  q.normalize(); r.normalize(); return make_pair(q,r/d);
}

bool BigInteger::sqrt(BigInteger &root) const {
  root.clear(); if (sign == 0) return true;
  BigInteger x, r; int d = mag.size(), root_d = (d+1)/2;
  r.sign = 1, root.sign = 1;
  
  if(d % 2 == 0) r.mag.push_back(mag[--d]);
  r <<= 1; r.mag[0] = mag[--d];
  
  for (int k = root_d - 1; k >= 0; k--) {
    x = root * 2;   x <<= 1;
    Digit lo = 0, hi = BASE;
    while (hi - lo > 1) {
      Digit mid = x.mag[0] = (lo + hi) / 2;
      (x*mid <= r ? lo : hi) = mid;
    }
    root <<= 1; root.mag[0] = x.mag[0] = lo; r -= x * lo;
    r <<= 1;  r += (d > 0) ? mag[--d] : 0;
    r <<= 1;  r += (d > 0) ? mag[--d] : 0;
  }
  return r.isZero();
}
