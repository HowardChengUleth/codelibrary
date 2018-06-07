#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>

using namespace std;


bool dEqual(int x, int y) { return x == y; }

struct Point {
  int x, y;
  bool operator==(const Point &p) const { return dEqual(x, p.x) && dEqual(y, p.y); }
  bool operator<(const Point &p) const { return x < p.x || (x == p.x && y < p.y); }
};

Point operator-(Point p,Point q){ p.x -= q.x; p.y -= q.y; return p; }
Point operator+(Point p,Point q){ p.x += q.x; p.y += q.y; return p; }
Point operator*(double r,Point p){ p.x *= r; p.y *= r; return p; }
int operator*(Point p,Point q){ return p.x*q.x + p.y*q.y; }
int len2(Point p){ return p*p; }
int cross(Point p,Point q){ return p.x*q.y - q.x*p.y; }
Point inv(Point p){ Point q = {-p.y,p.x}; return q; }


//----------------------------------------------------------
// Intersection of lines (line segment or infinite line)
//      (1 == 1 intersection pt, 0 == no intersection pts, -1 == infinitely many
int intersect_line(Point a, Point b, Point c, Point d, Point &p,bool segment=true) {
  double num1 = cross(d-c,a-c), num2 = cross(b-a,a-c),denom = cross(b-a,d-c);
  if (!dEqual(denom, 0)) {
    double r = num1 / denom, s = num2 / denom;
    if (!segment || (0 <= r && r <= 1 && 0 <= s && s <= 1)) {
      p = a + r*(b-a); return 1;
    } else return 0;
  }
  if(!segment) return dEqual(num1,0) ? -1 : 0; // For infinite lines, this is the end
  if (!dEqual(num1, 0)) return 0;
  if(b < a) swap(a,b); if(d < c) swap(c,d);
  if (a.x == b.x) {
    if (b.y == c.y) { p = b; return 1; }
    if (a.y == d.y) { p = a; return 1; }
    return (b.y < c.y || d.y < a.y) ? 0 : -1;
  } else if (b.x == c.x) { p = b; return 1; }
  else if (a.x == d.x) { p = a; return 1; }
  else if (b.x < c.x || d.x < a.x) return 0;
  return -1;
}

typedef pair<int,int> pii;

int s, r, w, p;

pii cell_index(const Point &p)
{
  return pii((p.x+10000)/(r+1), (p.y+10000)/(r+1));
}

typedef pair<Point, Point> Line;
Line wall[10];

bool can_see(const Point &p1, const Point &p2)
{
  Point p;
  int dist = r;
  for (int i = 0; i < w; i++) {
    if (intersect_line(p1, p2, wall[i].first, wall[i].second, p)) {
      dist--;
    }
  }
  return dist*dist >= len2(p1-p2);
}

void solve()
{
  cin >> s >> r >> w >> p;
  
  map<pii, vector<Point> > sensors;
  for (int i = 0; i < s; i++) {
    Point sensor;
    cin >> sensor.x >> sensor.y;
    pii loc = cell_index(sensor);
    sensors[loc].push_back(sensor);
  }

  for (int i = 0; i < w; i++) {
    cin >> wall[i].first.x >> wall[i].first.y
	>> wall[i].second.x >> wall[i].second.y;
  }

  for (int i = 0; i < p; i++) {
    Point prod;
    cin >> prod.x >> prod.y;

    pii loc = cell_index(prod);
    vector<Point> ans;

    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
	pii cell(loc.first + dx, loc.second + dy);
	if (sensors.find(cell) == sensors.end()) continue;
	vector<Point> &v = sensors[cell];
	for (unsigned int i = 0; i < v.size(); i++) {
	  if (can_see(v[i], prod)) {
	    ans.push_back(v[i]);
	  }
	}
      }
    }

    sort(ans.begin(), ans.end());
    cout << ans.size();
    for (unsigned int i = 0; i < ans.size(); i++) {
      cout << " (" << ans[i].x << "," << ans[i].y << ")";
    }
    cout << endl;
  }  
}

int main()
{
  int T;
  cin >> T;

  while (T--)
    solve();

  return 0;
}
