int gcd(int a, int b, int &s, int &t) { // a*s+b*t = g
  if (b==0) { t = 0; s = (a < 0) ? -1 : 1; return (a < 0) ? -a : a;
  } else { int g = gcd(b, a%b, t, s);  t -= a/b*s;  return g; }
}
