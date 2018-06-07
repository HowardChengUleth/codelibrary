/*
 * Convex Polygon Intersection
 *
 * Author: Howard Cheng
 *
 * This routine takes two convex polygon, and returns the intersection
 * which is also convex.  If the intersection contains less than
 * 3 points, it is considered empty.
 *
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

const double EPS = 1e-8;
bool dEqual(double x,double y) { return fabs(x-y) < EPS; }

struct Point {
  double x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return x < p.x || (x == p.x && y < p.y); }
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

//----------------------------------------------------------
// Point in polygon
const bool BOUNDARY = true;  // is boundary in polygon?
bool point_in_poly(Point poly[], int n, Point p) {
  int i, j, c = 0;
  for (i = 0; i < n; i++) 
    if (poly[i] == p || ccw(poly[i], poly[(i+1)%n], p) == CNEITHER) return BOUNDARY;
  
  for (i = 0, j = n-1; i < n; j = i++) 
    if (((poly[i].y <= p.y && p.y < poly[j].y) ||
         (poly[j].y <= p.y && p.y < poly[i].y)) &&
        (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) /
         (poly[j].y - poly[i].y) + poly[i].x))
      c = !c;
  return c;
}

//----------------------------------------------------------
// Intersection of lines (line segment or infinite line)
//      (1 == 1 intersection pt, 0 == no intersection pts, -1 == infinitely many
int intersect_line(Point a, Point b, Point c, Point d, Point &p,bool segment=true) {
  double num1 = cross(d-c,a-c), num2 = cross(b-a,a-c),denom = cross(b-a,d-c);
  if (!dEqual(denom, 0)) {
    double r = num1 / denom, s = num2 / denom;
    if (!segment || (0-EPS <= r && r <= 1+EPS && 0-EPS <= s && s <= 1+EPS)) {
      p = a + r*(b-a); return 1;
    } else return 0;
  }
  if(!segment) return dEqual(num1,0) ? -1 : 0; // For infinite lines, this is the end
  if (!dEqual(num1, 0)) return 0;
  if(b < a) swap(a,b); if(d < c) swap(c,d);
  if (a.x == b.x) {
    if (b.y == c.y) { p = b; return 1; }
    if (a.y == d.y) { p = a; return 1; }
    return (b.y < c.y || d.y < a.y) ? 0 : -1;
  } else if (b.x == c.x) { p = b; return 1; }
  else if (a.x == d.x) { p = a; return 1; }
  else if (b.x < c.x || d.x < a.x) return 0;
  return -1;
}

int intersect_polygon(Point poly1[], int n1, Point poly2[], int n2, 
		      Point *&out)
{
  Point p;
  char *used;
  int new_n = n1 + n2 + n1*n2;
  int count, i, i2, j, j2, new_count;
  int n;

  out = new Point[new_n];
  used = new char[new_n];
  assert(out && used);
  count = 0;
  fill(used, used+new_n, 0);

  for (i = 0; i < n1; i++) {
    if (point_in_poly(poly2, n2, poly1[i])) {
      out[count++] = poly1[i];
    }
  }
  for (i = 0; i < n2; i++) {
    if (point_in_poly(poly1, n1, poly2[i])) {
      out[count++] = poly2[i];
    }
  }

  for (i = 0; i < n1; i++) {
    i2 = (i+1 == n1) ? 0 : i+1;
    for (j = 0; j < n2; j++) {
      j2 = (j+1 == n2) ? 0 : j+1;
      if (intersect_line(poly1[i], poly1[i2], poly2[j], poly2[j2], p) == 1) {
        out[count++] = p;
      }
    }
  }

  n = count;
  
  /* eliminate duplicates */
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (out[i].x == out[j].x && out[i].y == out[j].y) {
        used[j] = 1;
      }
    }
  }
  j = 0;
  new_count = 0;
  for (i = 0; i < n; i++) {
    if (!used[i]) {
      out[new_count++] = out[i];
    }
  }
  n = new_count;

  delete[] used;
  return n;
}

int read_poly(Point *&poly)
{
  int n, i;

  cin >> n;
  if (n == 0) {
    return 0;
  }
  poly = new Point[n];
  assert(poly);
  for (i = 0; i < n; i++) {
    cin >> poly[i].x >> poly[i].y;
  }
  return n;

}

int main(void)
{
  Point *poly1, *poly2, *intersection;
  int n1, n2, n3, i;

  while ((n1 = read_poly(poly1)) >= 3) {
    if ((n2 = read_poly(poly2)) < 3) break;
    n3 = intersect_polygon(poly1, n1, poly2, n2, intersection);
    delete[] poly1;
    delete[] poly2;
    cout << n3 << endl;
    sort(intersection, intersection+n3);
    for (i = 0; i < n3; i++) {
      cout << fixed << setprecision(2);
      cout << intersection[i].x << " " << intersection[i].y << endl;
    }
    delete[] intersection;
  }

  return 0;
}
