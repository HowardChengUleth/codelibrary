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

//------------------------------------------------------------------------------
// Colinearity test
bool colinear(Point a, Point b, Point c) { return dEqual(cross(b-a,c-b),0); }

//------------------------------------------------------------------------------
// Orientation test  (When pts are colinear: ccw: a-b-c  cw: c-a-b  neither: a-c-b)
Orientation ccw(Point a, Point b, Point c) { // 
  Point d1 = b - a, d2 = c - b;
  if (dEqual(cross(d1,d2),0))
    if (d1.x * d2.x < 0 || d1.y * d2.y < 0) 
      return (d1 * d1 >= d2*d2 - EPS) ? CNEITHER : CW;
    else return CCW;
  else return (cross(d1,d2) > 0) ? CCW : CW;
}

//------------------------------------------------------------------------------
// Signed Area of Polygon
double area_polygon(Point p[], int n) {
  double sum = 0.0;
  for (int i = 0; i < n; i++)  sum += cross(p[i],p[(i+1)%n]);
  return sum/2.0;
}

//------------------------------------------------------------------------------
// Convex hull: Contains co-linear points. To remove colinear points:
//   Change ("< -EPS" and "> EPS") to ("< EPS" and "> -EPS")
int convex_hull(Point P[], int n, Point hull[]){
  sort(P,P+n); n = unique(P,P+n) - P;  vector<Point> L,U;
  if(n <= 2) { copy(P,P+n,hull); return n; }
  for(int i=0;i<n;i++){
    while(L.size()>1 && cross(P[i]-L.back(),L[L.size()-2]-P[i]) < -EPS) L.pop_back();
    while(U.size()>1 && cross(P[i]-U.back(),U[U.size()-2]-P[i]) >  EPS) U.pop_back();
    L.push_back(P[i]); U.push_back(P[i]);
  }
  copy(L.begin(),L.end(),hull); copy(U.rbegin()+1,U.rend()-1,hull+L.size());
  return L.size()+U.size()-2;
}

//------------------------------------------------------------------------------
// Point in Polygon Test
const bool BOUNDARY = true;  // is boundary in polygon?
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

//------------------------------------------------------------------------------
// Computes the distance from "c" to the infinite line defined by "a" and "b" 
double dist_line(Point a, Point b, Point c) { return fabs(cross(b-a,a-c)/len(b-a)); }

//------------------------------------------------------------------------------
// Intersection of lines (line segment or infinite line)
//      (1 == 1 intersection pt, 0 == no intersection pts, -1 == infinitely many
int intersect_line(Point a, Point b, Point c, Point d, Point &p,bool segment) {
  double num1 = cross(d-c,a-c), num2 = cross(b-a,a-c),denom = cross(b-a,d-c);
  if (!dEqual(denom, 0)) {
    double r = num1 / denom, s = num2 / denom;
    if (!segment || (0-EPS <= r && r <= 1+EPS && 0-EPS <= s && s <= 1+EPS)) {
      p = a + r*(b-a); return 1;
    } else return 0;
  }
  if (!segment) return dEqual(num1,0) ? -1 : 0; // For infinite lines, this is the end
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
// Intersect circle and line
// -> # of intersection points, in ans1 (and ans2)
struct Line{  Point a,b;  }; // distinct  points
int intersect_iline_circle(Line l,Circle c, Point& ans1, Point& ans2) {
  Point a = l.a - c.c, b = l.b - c.c; Point d = b - a;
  double dr = d*d, D = cross(a,b); double desc = SQR(c.r)*dr - SQR(D);
  if(dEqual(desc,0)){ ans1 = c.c-D/dr*inv(d); return 1; }
  if(desc < 0) return 0; double sgn = (d.y < -EPS ? -1 : 1);
  Point f = (sgn*sqrt(desc)/dr)*d; d = c.c-D/dr*inv(d);
  ans1 = d + f; ans2 = d - f; return 2;
}

//------------------------------------------------------------------------------
// Circle From Points
bool circle3pt(Point a, Point b, Point c, Point &center, double &r) {
  double g = 2*cross((b-a),(c-b)); if (dEqual(g, 0)) return false; // colinear points
  double e = (b-a)*(b+a)/g, f = (c-a)*(c+a)/g;
  center = inv(f*(b-a) - e*(c-a));
  r = len(a-center);
  return true;
}

//------------------------------------------------------------------------------
// Closest Pair of Points
Point M;
bool left_half(Point p){ return p.x<M.x || (p.x==M.x && p.y>M.y); }
double cp(Point P[],int n,vector<Point>& X,int l,int h){
  if(h - l == 2) return len(P[l]-P[l+1]);
  if(h - l == 3) return min(len(P[l]-P[l+1]),
                            min(len(P[l]-P[l+2]),len(P[l+1]-P[l+2])));
  M = X[(h+l)/2]; int m = stable_partition(P+l,P+h,left_half)-P;
  double d = min(cp(P,n,X,l,m),cp(P,n,X,m,h));
  M.x += d, M.y = LARGE_NUM; int t=stable_partition(P+m,P+h,left_half)-P;
  for(int i=l,j=m;i<m && j<t;i++){ if(P[m].x - P[i].x >= d) continue;
    while(j < t && P[i].y - P[j].y >= d) j++;
    for(int k=j;k<t && P[k].y-P[i].y < d;k++)
      if(len(P[k]-P[i]) < d) d=len(P[k]-P[i]);
  }
  inplace_merge(P+m,P+t,P+h); inplace_merge(P+l,P+m,P+h);
  return d;
}
double closest_pair(Point P[],int n){ // Call this from your program
  sort(P,P+n); if(n == 1) return -1; // Undefined
  Point* u = adjacent_find(P,P+n); if(u != P+n) return 0;
  vector<Point> X(n);      for(int i=0;i<n;i++) X[i]=inv(P[i]);
  sort(X.begin(),X.end()); for(int i=0;i<n;i++) X[i]=-1*inv(X[i]);
  return cp(P,n,X,0,n);
}

//------------------------------------------------------------------------------
// Minimum Enclosing Circle [Expected O(n) if you use the random_shuffle]
// inf needs to be bigger than the largest distance between points
Point tmp_c,pL,pR,mid; double tmp_r,inf=1e12;
bool all_of(Point* first,Point* last,bool (*f)(Point p)){
  for(;first != last;++first) if(!f(*first)) return false;
  return true;
}
bool in_circle(Point p){ return len(p-tmp_c) <= tmp_r + EPS; }
void circle2pt(Point a,Point b,Point& c,double& r){ c=0.5*(a+b); r=len(c-a); }
void minimum_enclosing_circle(Point P[],int N,Point& c,double& r){
  if(N <= 1) { c = P[0]; r = 0; return; } random_shuffle(P,P+N);
  circle2pt(P[0],P[1],c,r);
  
  for(int i=2;i<N;i++){
    if(len(c-P[i]) <= r + EPS) continue;
    circle2pt(P[0],P[i],c,r);
    for(int j=1;j<i;j++){
      if(len(c-P[j]) <= r + EPS) continue;
      circle2pt(P[i],P[j],mid,r); pL = pR = mid;
      
      double distL = -inf, distR = -inf;
      for(int k=0;k<j;k++)
	if(circle3pt(P[i],P[j],P[k],c,r)){
	  double dist = (ccw(P[i],mid,P[k]) == ccw(P[i],mid,c) ? 1 : -1)*len(mid-c);
	  if(ccw(P[i],mid,P[k]) == CCW && dist > distL) { pL = c; distL = dist; }
	  if(ccw(P[i],mid,P[k]) ==  CW && dist > distR) { pR = c; distR = dist; }
	}
      if(len(P[i]-pL) > len(P[i]-pR)) swap(pL,pR);
      c=tmp_c=mid; r=tmp_r=len(c-P[i]); if(all_of(P,P+j,in_circle)) continue;
      c=tmp_c=pL;  r=tmp_r=len(c-P[i]); if(all_of(P,P+j,in_circle)) continue;
      c=pR;        r=len(c-P[i]);
    }
  }
}

// ------------------------------------------------------------------------------
// Rotating Calipers, finds all anti-podal pairs in O(n)
// Note: need to update definition of Point, * operator, cross, and colinear
//       to use integers 
void calipers(vector<Point> &P) {
    auto nxt = [&](int a) { return (a+1) % P.size(); };
    auto calc = [&](int a, int b) {
                    /* P[a] and P[b] are an anti-podal pair, use them here */
                };
    auto colinear_ = [&](int i, int j) {
                         Point d = P[nxt(i)] - P[j];
                         return colinear(P[i], P[nxt(i)], P[nxt(j)] + d);
                     };
    auto ccw_ = [&](int i, int j) {
                    Point d = P[nxt(i)] - P[j];
                    return ccw(P[i], P[nxt(i)], P[nxt(j)] + d);
                };
    int i = 0, j = 1;
    while (colinear_(i, j)) j = nxt(j);
    while (ccw_(i, j) == CCW) j = nxt(j);
    do {
        calc(i, j);
        Orientation c = ccw_(i, j);
        if (colinear_(i, j)) { // (i, i+1) and (j,j+1) parallel
            calc(nxt(i), j);
            calc(i, nxt(j));
            j = nxt(j), i = nxt(i);
        }
        else if (c == CW) // parallel edges through (i,i+1) and j 
            i = nxt(i);
        else if (c == CCW) // parallel through (j,j+1) and i
            j = nxt(j);
    } while (j != 1);
}

