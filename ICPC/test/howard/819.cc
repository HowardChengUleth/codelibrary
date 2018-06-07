#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

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

#include <cfloat>

double box(Point poly[], int n, Point dir)
{
  double min_x, min_y, max_x, max_y;
  Point dir_comp;
  dir_comp.x = -dir.y;
  dir_comp.y = dir.x;
  
  for (int i = 0; i < n; i++) {
    double x = poly[i] * dir;
    double y = poly[i] * dir_comp;
    if (i == 0) {
      min_x = max_x = x;
      min_y = max_y = y;
    } else {
      min_x = min(min_x, x);
      max_x = max(max_x, x);
      min_y = min(min_y, y);
      max_y = max(max_y, y);
    }
  }

  return (max_x - min_x) * (max_y - min_y);
}

const double PI = acos(-1.0);

bool solve(int id)
{
  int n;
  cin >> n;
  if (!n) return false;

  Point poly[100], hull[100];
  for (int i = 0; i < n; i++) {
    cin >> poly[i].x >> poly[i].y;
  }
  n = convex_hull(poly, n, hull);

  double min_area = DBL_MAX, max_area = DBL_MIN;
  for (int i = 0; i < n; i++) {
    Point line = hull[i] - hull[(i+1)%n];
    line = 1.0/len(line) * line;
    double A = box(hull, n, line);
    min_area = min(min_area, A);
    max_area = max(max_area, A);
  }

  double steps = 100000;

  for (int i = 0; i < steps; i++) {
    double ang = i*PI/2/steps;
    Point dir;
    dir.x = cos(ang);  dir.y = sin(ang);
    double A = box(hull, n, dir);
    max_area = max(max_area, A);
  }

  
  cout << "Gift " << id << endl;
  cout << "Minimum area = " << setprecision(3) << fixed << min_area << endl;
  cout << "Maximum area = " << setprecision(3) << fixed << max_area << endl;
  cout << endl;
  
  return true;
}

int main()
{
  int id = 1;
  while (solve(id++))
    ;
  
  return 0;
}
