/* @JUDGE_ID: 1102NT 681 C "" */
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstdio>

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

  // remove colinear points
  for (int i = count-2; i >= 0; i--) {
    if (colinear(hull[i], hull[i+1], hull[(i+2)%count])) {
      for (int j = i+1; j < count-1; j++) {
	hull[j] = hull[j+1];
      }
      count--;
    }
  }
  
  return count;
}


int main(void)
{
  /* the statement about n <= 512 is wrong! */
  /* also the statement about no colinear points is wrong! */

  Point *polygon, *hull;
  int n, hull_size;
  int i, cases;

  scanf("%d", &cases);
  printf("%d\n", cases);
  while (cases-- > 0) {
    scanf("%d", &n);
    n--;
    polygon = new Point[n];
    hull = new Point[n];
    assert(polygon && hull);

    for (i = 0; i < n; i++) {
      scanf("%lf %lf", &polygon[i].x, &polygon[i].y);
    }
    hull_size = convex_hull(polygon, n, hull);
    printf("%d\n", hull_size+1);
    for (i = 0; i < hull_size; i++) {
      printf("%d %d\n", (int)floor(hull[i].x+0.5),
	     (int)floor(hull[i].y+0.5));
    }
    scanf("%*d %*d");
    printf("%d %d\n", (int)floor(hull[0].x+0.5), (int)floor(hull[0].y+0.5));

    if (cases) {
      scanf("%*d");
      printf("-1\n");
    }

    delete[] polygon;
    delete[] hull;
  }

  return 0;
}
