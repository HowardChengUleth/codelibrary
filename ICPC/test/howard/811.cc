
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


double dist(Point a, Point b)
{
  return len(a-b);
}

bool solve(int id)
{
  int n;
  cin >> n;
  if (!n) return false;
  
  Point tree[15];
  int val[15], len[15];
  for (int i = 0; i < n; i++) {
    cin >> tree[i].x >> tree[i].y >> val[i] >> len[i];
  }

  int best_subset;
  int best_val = -1;
  int best_num = -1;
  double best_excess = -1.0;
  for (int s = 1; s < (1 << n); s++) {
    Point poly[15], hull[15];
    int p = 0;
    int len_cut = 0;
    int val_cut = 0;
    for (int i = 0; i < n; i++) {
      if (s & (1 << i)) {
	poly[p++] = tree[i];
      } else {
	val_cut += val[i];
	len_cut += len[i];
      }
    }
    int h = convex_hull(poly, p, hull);
    double peri = 0.0;
    for (int i = 0; i < h; i++) {
      peri += dist(hull[i], hull[(i+1)%h]);
    }
    if (len_cut < peri) continue;
    if (best_val == -1 ||
	(val_cut < best_val) ||
	(val_cut == best_val && n-p < best_num)) {
      best_subset = s;
      best_val = val_cut;
      best_num = n-p;
      best_excess = len_cut - peri;
    }
  }

  if (id > 1) cout << endl;
  cout << "Forest " << id << endl;
  cout << "Cut these trees:";
  for (int i = 0; i < n; i++) {
    if (!(best_subset & (1 << i))) {
      cout << ' ' << i+1;
    }
  }
  cout << endl;
  cout << "Extra wood: " << fixed << setprecision(2) << best_excess << endl;
  
  return true;
}

int main(void)
{
  int id = 1;
  while (solve(id++))
    ;
  return 0;
}
