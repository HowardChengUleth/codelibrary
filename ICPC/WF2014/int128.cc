// 128-bit integers I/O routines (untested!)...does not work for -2^127
istream &operator>>(istream &is, __int128_t &x) {
  char c,r,neg=0;  x = 0;
  
  if(!(is >> c)) return is;
  if(!isdigit(c) || (c == '-') && !isdigit(is.peek()))
    is.setstate( ios::failbit );
  neg = (c == '-');
  if(!neg) is.putback(c);
  
  while (is && isdigit(is.peek()))
    is.get(c), x = x*10 + (c - '0');

  if (neg) x = -x;
  return is;
}

ostream &operator<<(ostream &os, __int128_t x) {
  if (x == 0) return os << 0;
  if (x < 0) { os << '-'; x *= -1; }
  int A[10], k = 0, M = 10000;
  while (x > 0) {  A[k++] = x % M;  x /= M;   }
  os << A[--k];
  while (k > 0) os << setw(4) << setfill('0') << A[--k];
  return os;
}

