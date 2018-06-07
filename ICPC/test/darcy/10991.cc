#include <iostream>
#include <cmath>
#include <iomanip>
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

//----------------------------------------------------------
// Intersect 2 circles: 3 -> infinity, or 0-2 intersection points
// Does not deal with radius of 0 (AKA points)
#define SQR(X) ((X) * (X))
struct Circle{ Point c; double r; };
int intersect_circle_circle(Circle c1,Circle c2,Point& ans1,Point& ans2) {
  if(c1.c == c2.c && dEqual(c1.r,c2.r)) return 3;
  double d = len(c1.c-c2.c);
  if(d > c1.r + c2.r + EPS || d < fabs(c1.r-c2.r) - EPS) return 0;
  double a = (SQR(c1.r) - SQR(c2.r) + SQR(d)) / (2*d);
  double h = sqrt(abs(SQR(c1.r) - SQR(a)));
  Point P = c1.c + a/d*(c2.c-c1.c);
  ans1 = P + h/d*inv(c2.c-c1.c); ans2 = P - h/d*inv(c2.c-c1.c);
  return dEqual(h,0) ? 1 : 2;
}

double area_heron(double a, double b, double c) { // assumes triangle valid
  return sqrt((a+b+c)*(c-a+b)*(c+a-b)*(a+b-c))/4.0;
}

const double PI = acos(-1.0);

int main(){
  cout << fixed << setprecision(6);
  int T;
  Point a,b,c,d;
  double R1,R2,R3;
  cin >> T;
  while(T--){
    cin >> R1 >> R2 >> R3;
    a.x = a.y = b.y = 0;
    b.x = R1 + R2;
    Circle c1 = {a,R1+R3};
    Circle c2 = {b,R2+R3};
    intersect_circle_circle(c1,c2,c,d);
    
    double theta1 = acos((1/(len(c-a)*len(b-a))) * ((c-a)*(b-a)));
    double theta2 = acos((1/(len(a-b)*len(c-b))) * ((a-b)*(c-b)));
    double theta3 = acos((1/(len(a-c)*len(b-c))) * ((a-c)*(b-c)));
        
    cout << area_heron(R1+R2,R2+R3,R1+R3) - 0.5*(theta1*(R1*R1) + theta2*(R2*R2) + theta3*(R3*R3)) << endl;
  }
  return 0;
}
