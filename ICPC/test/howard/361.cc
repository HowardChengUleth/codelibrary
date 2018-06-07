//Problem 361
//Written by Nathan House
//Started 1:21 Oct 24.
//Total time: 25mins

#include<iostream>
#include<iomanip>
#include<cmath>
#include<algorithm>
#include<cassert>
#include<string>
#include<vector>

using namespace std;

const double EPS = 1e-8;
bool dEqual(int x,int y) { return x == y; }

struct Point {
  int x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(double r,Point p){ p.x *= r; p.y *= r; return p; }
int operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
double len(Point p){ return sqrt(p*p); }
int cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
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
  if (n == 0) return 0;
  
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
  if (n <= 2) return false;
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

int numCit,numCop,numRob;
Point cits[200];
Point robs[200];
Point cops[200];

int main()
{
   int copHullsize=0;
   int robHullsize=0;
   Point copHull[200];
   Point robHull[200];
   int cases = 0;
   
   while(true)
   {
      cin >> numCop >> numRob >> numCit;
      if(numCop==0 && numRob==0 && numCit==0)
	 break;
      for(int i=0; i<numCop; i++)
	 cin >> cops[i].x >> cops[i].y;
      for(int i=0; i<numRob; i++)
	 cin >> robs[i].x >> robs[i].y;
      for(int i=0; i<numCit; i++)
	 cin >> cits[i].x >> cits[i].y;

      cout << "Data set " << cases+1 << ":" << endl;
      copHullsize=convex_hull(cops, numCop, copHull);
      robHullsize=convex_hull(robs, numRob, robHull);
      for(int j=0; j<numCit; j++)
      {
	 cout << "     Citizen at (" << cits[j].x << ","
	      << cits[j].y << ") is ";
	 if(point_in_poly(copHull, copHullsize, cits[j]))
	    cout << "safe." << endl;
	 else
	 {
	    if(point_in_poly(robHull, robHullsize, cits[j]))
	       cout << "robbed." << endl;
	    else
	    {
	       cout << "neither." << endl;
	    }
	 }
      }
      cout << endl;
      cases++;
   }
   return 0;
}

