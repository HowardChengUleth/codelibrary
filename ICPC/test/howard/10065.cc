#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

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
double area_polygon(Point p[], int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)
    sum += cross(p[i],p[(i+1)%n]);
  return sum/2.0;
}

//------------------------------------------------------------------------------
// Convex hull code: contains all colinear points on hull

Point start_p, max_p; // globals for convex hull only
bool ccw_cmp(Point a, Point b) { return ccw(start_p, a, b) == CCW; }

bool sort_cmp(Point a,Point b) {
  if (colinear(start_p, a, max_p) && colinear(start_p, b, max_p)) {
    Point d1 = start_p - a, d2 = start_p - b;
    d1.x = abs(d1.x); d2.x = abs(d2.x); d1.y = abs(d1.y); d2.y = abs(d2.y);
    return d2 < d1;
  } else return ccw_cmp(a,b);
}

int convex_hull(Point poly[], int n, Point hull[]) {
  sort(poly, poly+n);
  n = unique(poly, poly+n) - poly;
  if (n == 1) {  hull[0] = poly[0];  return 1;  }

  start_p = poly[0]; max_p = poly[1];
  for (int i = 2; i < n; i++) if (ccw_cmp(max_p, poly[i])) max_p = poly[i];
  sort(poly+1, poly+n, sort_cmp);

  int count = 2;     copy(poly, poly+2, hull);
  for (int i = 2; i < n; i++) {
    while (count > 1 && ccw(hull[count-2], hull[count-1], poly[i]) == CW) count--;
    hull[count++] = poly[i];
  }
  return count;
}

int main(void)
{
  int case_num = 1;
  int n;

  while (cin >> n && n > 0) {
    Point polygon[100];
    for (int i = 0; i < n; i++) {
      cin >> polygon[i].x >> polygon[i].y;
    }

    double tile_area = fabs(area_polygon(polygon, n));

    Point hull[100];
    int hull_size;
    hull_size = convex_hull(polygon, n, hull);

    double hull_area = fabs(area_polygon(hull, hull_size));

    cout << "Tile #" << case_num++ << endl;
    cout << "Wasted Space = " << setprecision(2) << fixed
	 << (hull_area - tile_area)*100 / hull_area << " %" << endl;
    cout << endl;
  }
  return 0;
}

