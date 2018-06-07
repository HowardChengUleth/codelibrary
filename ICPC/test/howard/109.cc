/* @JUDGE_ID: 1102NT 109 C "" */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

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
//----------------------------------------------------------
// Point in polygon
const bool BOUNDARY = false;  // is boundary in polygon?
bool point_in_poly(Point poly[], int n, Point p) {
  int i, j, c = 0;
  for (i = 0; i < n; i++) 
    if (poly[i] == p ||	ccw(poly[i], poly[(i+1)%n], p) == CNEITHER) return BOUNDARY;
  
  for (i = 0, j = n-1; i < n; j = i++) 
    if (((poly[i].y <= p.y && p.y < poly[j].y) ||
	 (poly[j].y <= p.y && p.y < poly[i].y)) &&
        (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) /
	 (poly[j].y - poly[i].y) + poly[i].x))
      c = !c;
  return c;
}

// Signed Area of polygon ------------------------------------------------------
double area_polygon(Point p[], int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)
    sum += cross(p[i],p[(i+1)%n]);
  return sum/2.0;
}

#define MAX_KINGDOM 20
#define MAX_VERTICES 100

typedef struct {
  int n;
  Point v[MAX_VERTICES];
  int hit;
} Kingdom;

Kingdom kingdom[MAX_KINGDOM];
int num_kingdoms;

double process_missle(Point p)
{
  int i;
  double sum = 0.0;
  int count = 0;

  for (i = 0; i < num_kingdoms; i++) {
    if (!kingdom[i].hit && point_in_poly(kingdom[i].v, kingdom[i].n, p)) {
      count++;
      sum += fabs(area_polygon(kingdom[i].v, kingdom[i].n));
      kingdom[i].hit = 1;
    }
  }
  assert(sum == 0.0 || count == 1);
  return sum;
}

int main(void)
{
  Point poly[MAX_VERTICES], missle;
  int N;
  int i;
  double area = 0.0;

  num_kingdoms = 0;
  while (scanf("%d", &N) == 1 && N != -1) {
    /* read kingdom */
    for (i = 0; i < N; i++) {
      scanf("%lf %lf", &poly[i].x, &poly[i].y);
    }

    /* compute convex hull */
    kingdom[num_kingdoms].n = convex_hull(poly, N, kingdom[num_kingdoms].v);
    kingdom[num_kingdoms].hit = 0;
    num_kingdoms++;
  }

  /* process missles */
  while (scanf("%lf %lf", &missle.x, &missle.y) == 2) {
    area += process_missle(missle);
  }
  printf("%0.2f\n", area);
  return 0;
}
