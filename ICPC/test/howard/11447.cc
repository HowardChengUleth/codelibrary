#include <iostream>
#include <iomanip>
#include <cmath>
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
double area_polygon(Point p[], int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)
    sum += cross(p[i],p[(i+1)%n]);
  return sum/2.0;
}

void solve()
{
  Point poly[10000];
  int N;
  cin >> N;
  for (int i = 0; i < N; i++) {
    cin >> poly[i].x >> poly[i].y;
  }
  int W;
  cin >> W;

  double init, used, fall;
  cin >> init >> used >> fall;

  double total = area_polygon(poly, N) * W;
  double V = total * init/100.0;

  // use it
  V -= used;
  if (V < 0) {
    V = 0;
    cout << "Lack of water. ";
  }

  // fill it
  V += fall;
  if (V > total) {
    V = total;
    cout << "Excess of water. ";
  }

  cout << "Final percentage: " << (int)(V/total*100) << "%" << endl;
  
}

int main(void)
{
  int T;
  cin >> T;
  while (T--)
    solve();
  return 0;
}

