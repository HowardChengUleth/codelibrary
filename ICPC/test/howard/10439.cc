/*
 * Parameters of circle from 3 points
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/
 *
 * This routine computes the parameters of a circle (center and radius)
 * from 3 points.  Returns non-zero if successful, zero if the three
 * points are colinear.
 *
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

using namespace std;
const double EPS = 1e-5;
bool dEqual(double x,double y) { return fabs(x-y) < EPS; }

struct Point {
  double x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y
); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x)
; }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(double r,Point p){ p.x *= r; p.y *= r; return p; }
double operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
double len(Point p){ return sqrt(p*p); }
double cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }


bool circle3pt(Point a, Point b, Point c, Point &center, double &r) {
  double g = 2*cross((b-a),(c-b)); if (dEqual(g, 0)) return false; // colinear points
  double e = (b-a)*(b+a)/g, f = (c-a)*(c+a)/g;
  center = inv(f*(b-a) - e*(c-a));
  r = len(a-center);
  return true;
}

// angle between ab and ac
double angle(Point a, Point b, Point c)
{
  b.x -= a.x; b.y -= a.y;
  c.x -= a.x; c.y -= a.y;
  double dot = b.x*c.x + b.y*c.y;
  double blen = sqrt(b.x*b.x + b.y*b.y);
  double clen = sqrt(c.x*c.x + c.y*c.y);
  double arg = dot/(blen*clen);
  arg = max(-1.0, arg);
  arg = min(1.0, arg);
  return acos(arg);
}

double distInt(double x)
{
  return fabs(floor(x+0.5) - x);
}

void solve()
{
  Point a, b, c, center;
  double r;

  cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y;
  circle3pt(a, b, c, center, r);

  double ang1 = angle(center, a, b);
  double ang2 = angle(center, b, c);
  double PI = acos(-1.0);
  for (int n = 3; n <= 200; n++) {
    double dist1 = distInt(ang1/(2*PI/n));
    double dist2 = distInt(ang2/(2*PI/n));
    if (dist1 < EPS && dist2 < EPS) {
      cout << n << endl;
      return;
    }
  }
}

int main()
{
  int n;
  cin >> n;
  while (n--) {
    solve();
  }
  return 0;
}

