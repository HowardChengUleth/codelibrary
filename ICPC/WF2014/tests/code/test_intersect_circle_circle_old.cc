// Determines the point(s) of intersection if a circle and a circle
//
// Author: Darcy Best
// Date  : October 1, 2010
// Source: http://local.wasp.uwa.edu.au/~pbourke/geometry/2circle/
//
// Note: A circle of radius 0 must be considered independently.
// See comments in the implementation.

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

#define SQR(X) ((X) * (X))

// How close to call equal
const double EPS = 1e-4;

bool dEqual(double x,double y){
  return fabs(x-y) < EPS;
}

struct Point{
  double x,y;
  bool operator<(const Point& a) const{
    if(dEqual(x,a.x))
      return y < a.y;
    return x < a.x;
  }
};

// Prints out the ordered pair. This also accounts for the negative 0.
void print(const Point& a){
  cout << "(";
  if(fabs(a.x) < 1e-4)
    cout << "0.000";
  else
    cout << a.x;
  cout << ",";
  if(fabs(a.y) < 1e-4)
    cout << "0.000";
  else
    cout << a.y;
  cout << ")";
}

struct Circle{
  double r,x,y;
};

// Input:
//  Two circles to intersect
//
// Output:
//  Number of points of intersection points
//  If 1 (or 2), then ans1 (and ans2) contain those points.
//  If 3, then there are infinitely many. (They're the same circle)
int intersect_circle_circle(Circle c1,Circle c2,Point& ans1,Point& ans2){
	
  // If we have two singular points
  if(fabs(c1.r) < EPS && fabs(c2.r) < EPS){
    if(dEqual(c1.x,c2.x) && dEqual(c1.y,c2.y)){
      ans1.x = c1.x;
      ans1.y = c1.y;
      // Here, you need to know what the intersection of two exact points is:
      //  "return 1;" - If the points intersect at only 1 point
      //  "return 3;" - If the circles are the same
      // Note that both are true -- It all depends on the problem
      return 1;
    } else {
      return 0;
    }
  }
	
  double d = hypot(c1.x-c2.x,c1.y-c2.y);
	
  // Check if the circles are exactly the same.
  if(dEqual(c1.x,c2.x) && dEqual(c1.y,c2.y) && dEqual(c1.r,c2.r))
    return 3;
	
  // The circles are disjoint
  if(d > c1.r + c2.r + EPS)
    return 0;
	
  // One circle is contained inside the other -- No intersection
  if(d < abs(c1.r-c2.r) - EPS)
    return 0;
	
  double a = (SQR(c1.r) - SQR(c2.r) + SQR(d)) / (2*d);
  double h = sqrt(abs(SQR(c1.r) - SQR(a)));
	
  Point P;
  P.x = c1.x + a / d * (c2.x - c1.x);
  P.y = c1.y + a / d * (c2.y - c1.y);
	
  ans1.x = P.x + h / d * (c2.y - c1.y);
  ans1.y = P.y - h / d * (c2.x - c1.x);
	
  if(fabs(h) < EPS)
    return 1;
	
  ans2.x = P.x - h / d * (c2.y - c1.y);
  ans2.y = P.y + h / d * (c2.x - c1.x);
	
  return 2;
}

int main(){
  cout << fixed << setprecision(3);
  Circle C1,C2;
  Point a1,a2;
	
  while(cin >> C1.x >> C1.y >> C1.r >> C2.x >> C2.y >> C2.r){
    int num = intersect_circle_circle(C1,C2,a1,a2);
    if(a1.x < a2.x || (a1.x == a2.x && a1.y < a2.y)) swap(a1,a2);
    switch(num){
    case 0:
      cout << "NO INTERSECTION" << endl;
      break;
    case 1:
      print(a1); cout << endl;
      break;
    case 2:
      print(a1);print(a2);cout << endl;
      break;
    case 3:
      cout << "THE CIRCLES ARE THE SAME" << endl;
      break;
    }
  }
  return 0;
}
