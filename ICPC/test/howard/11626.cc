#include <iostream>
#include <cmath>

using namespace std;

#include <algorithm>

using namespace std;

typedef long long ll;


bool dEqual(ll x,ll y) { return x==y; }

struct Point {
  ll x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return x < p.x || (x == p.x && y < p.y); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(double r,Point p){ p.x *= r; p.y *= r; return p; }
ll operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
double len(Point p){ return sqrt(p*p); }
ll cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }

enum Orientation {CCW, CW, CNEITHER};

bool colinear(Point a, Point b, Point c) { return dEqual(cross(b-a,c-b),0); }

Orientation ccw(Point a, Point b, Point c) { // ccw: a-b-c  cw: c-a-b  neither: a-c-b
  Point d1 = b - a, d2 = c - b;
  if (dEqual(cross(d1,d2),0))
    if (d1.x * d2.x < 0 || d1.y * d2.y < 0) 
      return (d1 * d1 >= d2*d2) ? CNEITHER : CW;
    else return CCW;
  else return (cross(d1,d2) > 0) ? CCW : CW;
}


istream &operator>>(istream &is, Point &p)
{
  return is >> p.x >> p.y;
}

ostream &operator<<(ostream &os, const Point &p)
{
  return os << p.x << ' ' << p.y;
}

Point p[100000];
int num_p;

Point p_min, p_max;

bool cmp_angle(const Point &p1, const Point &p2)
{
  return (ccw(p_min, p1, p2) == CCW);
}

bool cmp_sort(const Point &p1, const Point &p2)
{
  if (colinear(p_min, p1, p_max) && colinear(p_min, p2, p_max)) {
    // p0 -> p1 and p0 -> p2 colinear with p0 -> p_max, farther one comes first
    ll dx1 = abs(p_min.x - p1.x);
    ll dx2 = abs(p_min.x - p2.x);
    ll dy1 = abs(p_min.y - p1.y);
    ll dy2 = abs(p_min.y - p2.y);
    return dx1 > dx2 || (dx1 == dx2 && dy1 > dy2);
  } else {
    return (ccw(p_min, p1, p2) == CCW);
  }
}

void do_case()
{
  int n;
  cin >> n;

  num_p = 0;
  for (int i = 0; i < n; i++) {
    char c;
    cin >> p[num_p] >> c;
    if (c == 'Y') {
      num_p++;
    }
  }

  // find the min
  int min_index = 0;
  for (int i = 1; i < num_p; i++) {
    if (p[i] < p[min_index]) {
      min_index = i;
    }
  }
  swap(p[0], p[min_index]);
  p_min = p[0];
  
  // find the maximum "angle" wrt to p[0] and positive x-axis
  int max_index = 1;
  for (int i = 2; i < num_p; i++) {
    if (cmp_angle(p[max_index], p[i])) {
      max_index = i;
    }
  }
  p_max = p[max_index];

  
  sort(p+1, p+num_p, cmp_sort);

  // output
  cout << num_p << endl;
  for (int i = 0; i < num_p; i++) {
    cout << p[i] << endl;
  }


}

int main(void)
{
  int T;
  cin >> T;
  while (T-- > 0) {
    do_case();
  }
  return 0;
}
