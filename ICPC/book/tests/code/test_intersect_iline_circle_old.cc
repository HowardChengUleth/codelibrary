// Determines the point(s) of intersection if a circle and a line
//
// Author: Darcy Best
// Date  : May 1, 2010
// Source: http://mathworld.wolfram.com/Circle-LineIntersection.html

#include <iostream>
#include <cmath>
using namespace std;

#define SQR(X) ((X) * (X))

// How close to call equal
const double EPS = 1e-7;

bool dEqual(double x,double y){
  return fabs(x-y) < EPS;
}

struct Point{
  double x,y;
};

struct Line{
  Point p1,p2;
};

struct Circle{
  Point centre;
  double radius;
};

// Input of:
//  - 2 distinct points on the line
//  - The centre of the circle
//  - The radius of the circle
// Output:
//  Number of points of intersection points
//  If 1 or 2, then ans1 and ans2 contain those points.
int intersect_iline_circle(Line l,Circle c,Point& ans1,Point& ans2){
  Point p1 = l.p1;
  Point p2 = l.p2;
	
  Point circCentre = c.centre;
  double rad = c.radius;
	
  p1.x -= circCentre.x;
  p2.x -= circCentre.x;
  p1.y -= circCentre.y;
  p2.y -= circCentre.y;
	
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;
  double dr = SQR(dx) + SQR(dy);
  double D  = p1.x*p2.y - p2.x*p1.y;
	
  double desc = SQR(rad)*dr - SQR(D);
	
  if(dEqual(desc,0)){
    ans1.x = (D*dy) / dr;
    ans1.y = (-D*dx) / dr;
    return 1;
  } else if(desc < 0){
    return 0;
  }
	
  double sgn = (dy < -EPS ? -1 : 1);
	
  ans1.x = circCentre.x + (D*dy + sgn*dx*sqrt(desc)) / dr;
  ans1.y = circCentre.y + (-D*dx + abs(dy)*sqrt(desc)) / dr;
	
  ans2.x = circCentre.x + (D*dy - sgn*dx*sqrt(desc)) / dr;
  ans2.y = circCentre.y + (-D*dx - abs(dy)*sqrt(desc)) / dr;
	
  return 2;
}

int main(){
  Line L;
  Circle C;
  Point a1,a2;
  while(cin >> L.p1.x >> L.p1.y >> L.p2.x >> L.p2.y
	>> C.centre.x >> C.centre.y >> C.radius){
    int num = intersect_iline_circle(L,C,a1,a2);
    if(num == 0)
      cout << "NO INTERSECTION." << endl;
    else if(num == 1)
      cout << "ONE INTERSECTION:  (" << a1.x << "," << a1.y << ")" << endl;
    else
      cout << "TWO INTERSECTIONS: (" << a1.x << "," << a1.y << ")" 
	   << "(" << a2.x << "," << a2.y << ")" << endl;
  }
  return 0;
}
