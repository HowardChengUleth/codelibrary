/* @JUDGE_ID: 1102NT 10456 C "" */

#include <cstdlib>
#include <cassert>

#include <cstdio>
#include <cassert>

#include <cmath>
#include <algorithm>
using namespace std;

const long long EPS = 1e-8;
bool dEqual(long long x,long long y) { return x == y; }

struct Point {
  long long x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(long long r,Point p){ p.x *= r; p.y *= r; return p; }
long long operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
long long len(Point p){ return sqrt(p*p); }
long long cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }

enum Orientation {CCW, CW, CNEITHER};

bool colinear(Point a, Point b, Point c) { return dEqual(cross(b-a,c-b),0); }

Orientation ccw(Point a, Point b, Point c) { // ccw: a-b-c  cw: c-a-b  neither: a-c-b
  Point d1 = b - a, d2 = c - b;
  if (dEqual(cross(d1,d2),0))
    if (d1.x * d2.x < 0 || d1.y * d2.y < 0) 
      return (d1 * d1 >= d2*d2 - EPS) ? CNEITHER : CW;
    else return CCW;
  else return (cross(d1,d2) > 0) ? CCW : CW;
}

// Signed Area of polygon ------------------------------------------------------
long long area_polygon2(Point p[], int n) {
  long long sum = 0;
  for (int i = 0; i < n; i++)
    sum += cross(p[i],p[(i+1)%n]);
  return sum > 0 ? sum : -sum;
}

#define MAX_N 30

bool between(Point a, Point b, Point c)
{
  if (a == c || b == c) return true;
  return ccw(a, b, c) == CNEITHER;
}

// Note: Applying operations to reduced fractions should yield a reduced answer
//  EXCEPT: 0 may be 0/x until reduce is called (then changed to 0/1)
typedef long long ll;
struct frac{  ll num,den; };

ll gcd(ll a, ll b) { // a*s+b*t = g
  if (b==0) { return (a < 0) ? -a : a;
  } else { return gcd(b, a%b); }
}


frac make_frac(ll n,ll d){ frac f; f.num = n,f.den = d; return f; }

frac reduce(frac a){
  if(a.num == 0) return make_frac(0,1); if(a.den < 0) { a.num *= -1; a.den *= -1; }
  ll g = gcd(a.num,a.den); return make_frac(a.num/g,a.den/g);
}
frac recip(frac a){ return make_frac(a.den,a.num); }

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

bool operator==(frac a,frac b){ reduce(a);reduce(b); return a.num==b.num && a.den==b.den;}

// Choose one. First one may overflow. Second one has rounding errors.
bool operator<(frac a,frac b){ return (a.num*b.den) < (b.num*a.den); }

int read_poly(Point *poly)
{
  int n, i;
  if (scanf("%d", &n) != 1) {
    return 0;
  }

  for (i = 0; i < n; i++) {
    scanf("%lld %lld", &poly[i].x, &poly[i].y);
  }

  return n;
}

void solve(Point *poly, int n, Point p)
{
  Point hull[MAX_N+1];
  int i, j, pi;
  int A2 = area_polygon2(poly, n);
  ll tA, diff;

  for (pi = 1; pi < n; pi++) {
    if (between(poly[pi-1], poly[pi], p)) {
      break;
    }
  }
  if (pi == n) {
    assert(between(poly[0], poly[n-1], p));
  }
  j = 0;
  hull[j++] = p;
  i = pi % n;
  do {
    hull[j++] = poly[i];
    i = (i+1) % n;
  } while (i != pi % n);

  for (j = 3; (diff = 2*area_polygon2(hull, j)- A2) < 0; j++)
    ;

  frac x, y;
  if (!diff) {
    x = make_frac(hull[j-1].x, 1);
    y = make_frac(hull[j-1].y, 1);
  } else {
    tA = area_polygon2(hull,j) - area_polygon2(hull, j-1);
    x = make_frac(2*tA*hull[j-1].x + diff*(hull[j-2].x-hull[j-1].x), 2*tA);
    y = make_frac(2*tA*hull[j-1].y + diff*(hull[j-2].y-hull[j-1].y), 2*tA);
  }
  x = reduce(x);
  y = reduce(y);
  printf("(%lld/%lld),(%lld/%lld)\n", x.num, x.den, y.num, y.den);

}

int main(void)
{
  int m, n;
  Point poly[MAX_N+1], p;

  while (n = read_poly(poly)) {
    scanf("%d", &m);
    while (m-- > 0) {
      scanf("%lld %lld", &p.x, &p.y);
      solve(poly, n, p);
    }
  }

  return 0;
}
