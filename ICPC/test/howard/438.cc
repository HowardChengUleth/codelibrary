/* @JUDGE_ID: 1102NT 438 C "" */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>

const double EPS = 1e-8;
bool dEqual(double x,double y) { return fabs(x-y) < EPS; }

struct Point {
  double x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
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

int main(void)
{
    Point p1, p2, p3, center;
    double r, pi;

    pi = atan(1)*4;
    while (scanf("%lf %lf %lf %lf %lf %lf", &p1.x, &p1.y, &p2.x, &p2.y,
		 &p3.x, &p3.y) == 6) {
	circle3pt(p1, p2, p3, center, r);
	printf("%0.2f\n", 2*r*pi);
    }
    return 0;
}
