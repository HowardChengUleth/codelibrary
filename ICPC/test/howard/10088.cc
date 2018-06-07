#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

using namespace std;

bool dEqual(long long x,long long y) { return x == y; }

struct Point {
  long long x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return y < p.y || (y == p.y && x < p.x); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(long long r,Point p){ p.x *= r; p.y *= r; return p; }
long long operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
long long len(Point p){ return sqrt(p*p); }
long long cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }

enum Orientation {CCW, CW, CNEITHER};


// Signed Area of polygon ------------------------------------------------------
long long area_polygon2(Point p[], int n) {
  long long sum = 0;
  for (int i = 0; i < n; i++)
    sum += cross(p[i],p[(i+1)%n]);
  return sum;
}

int gcd(int a, int b)
{
  if (a < 0) return gcd(-a, b);
  if (b < 0) return gcd(a, -b);
  while (b) {
    int r = a % b;
    a = b;
    b = r;
  }
  return a;
}

int main(void)
{
  Point poly[1000];
  int n;

  while (cin >> n && n > 0) {
    for (int i = 0; i < n; i++) {
      cin >> poly[i].x >> poly[i].y;
    }

    long long A2 = area_polygon2(poly, n);
    if (A2 < 0) A2 *= -1;
    int B = 0;
    for (int i = 0; i < n; i++) {
      int dx = poly[i].x - poly[(i+1)%n].x;
      int dy = poly[i].y - poly[(i+1)%n].y;
      B += gcd(dx, dy);
    }
    cout << (A2 + 2 - B)/2 << endl;
  }
  return 0;
}

