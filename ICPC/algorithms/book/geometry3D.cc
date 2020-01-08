const double PI = acos(-1.0), EPS = 1e-8;

struct Vector {
  double x, y, z;
  Vector(double xx = 0, double yy = 0, double zz = 0) : x(xx), y(yy), z(zz) { }
  Vector(const Vector &p1, const Vector &p2)
    : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) { }
  Vector(const Vector &p1, const Vector &p2, double t)
    : x(p1.x + t*p2.x), y(p1.y + t*p2.y), z(p1.z + t*p2.z) { }
  double norm() const { return sqrt(x*x + y*y + z*z); }
  bool operator==(const Vector&p) const{
    return abs(x - p.x) < EPS && abs(y - p.y) < EPS && abs(z - p.z) < EPS;
  }
};

double dot(Vector p1, Vector p2) { return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z; }
double angle(Vector p1,Vector p2) {  return acos(dot(p1, p2)/p1.norm()/p2.norm()); }
Vector cross(Vector p1, Vector p2) {
  return Vector(p1.y*p2.z-p2.y*p1.z, p2.x*p1.z-p1.x*p2.z, p1.x*p2.y-p2.x*p1.y);
}
Vector operator+(Vector p1,Vector p2){ return Vector(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z); }
Vector operator-(Vector p1,Vector p2){ return Vector(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z); }
Vector operator*(double c,Vector v){ return Vector(c*v.x, c*v.y, c*v.z); }

double dist_pt_to_pt(Vector p1,Vector p2) { return Vector(p1, p2).norm(); }

// distance from p to the line segment defined by a and b
double dist_pt_to_segment(Vector p,Vector a,Vector b) {
  Vector u(a, p), v(a, b); double s = dot(u,v) / dot(v,v);
  if (s < 0 || s > 1) return min(dist_pt_to_pt(p, a), dist_pt_to_pt(p, b));
  return dist_pt_to_pt(Vector(a, v, s), p);
}

// distance from p to the infinite line defined by a and b
double dist_pt_to_line(Vector p, Vector a,Vector b) {
  Vector u(a, p), v(a, b); double s = dot(u,v) / dot(v,v);
  return dist_pt_to_pt(Vector(a, v, s), p);
}

// distance from p to the triangle defined by a, b, c
double dist_pt_to_triangle(Vector p, Vector a, Vector b, Vector c) {
  Vector u(a, p), v1(a, b), v2(a, c); Vector normal = cross(v1, v2);
  double s = dot(u, normal) / (normal.norm() * normal.norm());
  Vector proj(p, normal, -s);
  Vector wa(proj, a), wb(proj, b), wc(proj, c);
  double a1 = angle(wa, wb), a2 = angle(wa, wc), a3 = angle(wb, wc);
  if (fabs(a1 + a2 + a3 - 2*PI) < EPS) return dist_pt_to_pt(proj, p);
  return min(dist_pt_to_segment(p, a, b), min(dist_pt_to_segment(p, a, c),
					      dist_pt_to_segment(p, b, c)));
}

// distance from p to the infinite plane defined by a, b, c
double dist_pt_to_plane(Vector p, Vector a, Vector b, Vector c) {
  Vector u(a, p), v1(a, b), v2(a, c); Vector normal = cross(v1, v2);
  double s = dot(u, normal) / (normal.norm() * normal.norm());
  return dist_pt_to_pt(Vector(p, normal, -s), p);
}

// distance from segment p1->q1 to p2->q2
double dist_segment_to_segment(Vector p1, Vector q1, Vector p2, Vector q2) {
  Vector v1(p1, q1), v2(p2, q2);
  Vector rhs(dot(v1, p2) - dot(v1, p1), dot(v2, p1) - dot(v2, p2));
  double det = v1.norm()*v1.norm()*v2.norm()*v2.norm() - dot(v1, v2)*dot(v1, v2);
  if (det > EPS){
    double t = (rhs.x*v2.norm()*v2.norm() + rhs.y * dot(v1, v2)) / det;
    double s = (v1.norm()*v1.norm()*rhs.y + dot(v1, v2) * rhs.x) / det;
    if (0 <= s && s <= 1 && 0 <= t && t <= 1) 
      return dist_pt_to_pt(Vector(p1, v1, t), Vector(p2, v2, s));
  }
  return min(min(dist_pt_to_segment(p1, p2, q2), dist_pt_to_segment(q1, p2, q2)),
	     min(dist_pt_to_segment(p2, p1, q1), dist_pt_to_segment(q2, p1, q1)));
}

// distance from infinite lines defined by p1->q1 and p2->q2
double dist_line_to_line(Vector p1, Vector q1, Vector p2, Vector q2) {
  Vector v1(p1, q1), v2(p2, q2);
  Vector rhs(dot(v1, p2) - dot(v1, p1), dot(v2, p1) - dot(v2, p2));
  double det = v1.norm()*v1.norm()*v2.norm()*v2.norm() - dot(v1, v2)*dot(v1, v2);
  if (det < EPS) return dist_pt_to_line(p1, p2, q2);
  double t = (rhs.x*v2.norm()*v2.norm() + rhs.y * dot(v1, v2)) / det;
  double s = (v1.norm()*v1.norm()*rhs.y + dot(v1, v2) * rhs.x) / det;
  return dist_pt_to_pt(Vector(p1, v1, t), Vector(p2, v2, s));
}

// Rotate a point (P) around a line (defined by two points L1 and L2) by theta
//   Note: Rotation is counterclockwise when looking through L2 to L1.
Point rotate(Point P,Point L1,Point L2,double theta){
  double a=L1.x,b=L1.y,c=L1.z, u=(L2-L1).x,v=(L2-L1).y,w=(L2-L1).z;
  double x=P.x,y=P.y,z=P.z,L = sqrt(u*u+v*v+w*w); u /= L, v /= L, w /= L;
  double C=cos(theta),S=sin(theta),D=1-cos(theta),E=u*x+v*y+w*z;
  
  Point ans;
  ans.x = D*(a*(v*v+w*w) - u*(b*v+c*w-E)) + x*C + S*(b*w-c*v-w*y+v*z);
  ans.y = D*(b*(u*u+w*w) - v*(a*u+c*w-E)) + y*C + S*(c*u-a*w+w*x-u*z);
  ans.z = D*(c*(u*u+v*v) - w*(a*u+b*v-E)) + z*C + S*(a*v-b*u-v*x+u*y);

  return ans;
}

// 3D Convex Hull -- O(n^2)
//  -- To use:
//   vector<Vector> pts;
//   vector<hullFinder::hullFace> hull = hullFinder(pts).findHull();
//  -- Each entry in hull will represent indices of a triangle on the hull (u,v,w)
//  -- Some points may be coplanar
Vector tNorm(Vector a,Vector b,Vector c) { return cross(a,b)+cross(b,c)+cross(c,a); }
const Vector Zero;

class hullFinder {
  const vector<Vector> &pts;
public:
  hullFinder(const vector<Vector> &PTS) : pts(PTS), halfE(pts.size(),-1) {}
  struct hullFace {
    int u, v, w; Vector n;
    hullFace(int U, int V, int W, const Vector &N) : u(U), v(V), w(W), n(N) {}
  };
  vector<hullFinder::hullFace> findHull() {
    vector<hullFace> hull; int n = pts.size(), p3, p4; Vector t; edges.clear();
    if (n < 4) return hull;   // Not enough points  (hull is empty)
    for(p3 = 2  ; (p3 < n) && (t=tNorm(pts[0], pts[1], pts[p3])) == Zero ; p3++) {}
    for(p4=p3+1 ; (p4 < n) && (abs(dot(t, pts[p4] - pts[0])) < EPS)      ; p4++) {}
    if (p4 >= n) return hull; // All points coplanar (hull is empty)
    
    edges.push_front(hullEdge(0, 1)),setF1(edges.front(),p3),setF2(edges.front(),p3);
    edges.push_front(hullEdge(1,p3)),setF1(edges.front(), 0),setF2(edges.front(), 0);
    edges.push_front(hullEdge(p3,0)),setF1(edges.front(), 1),setF2(edges.front(), 1);
    addPt(p4); for (int i = 2; i < n; ++i) if ((i != p3) && (i != p4)) addPt(i);
    for (list<hullEdge>::iterator e = edges.begin(); e != edges.end(); ++e){
      if((e->u < e->v) && (e->u < e->f1))
	hull.push_back(hullFace(e->u, e->v, e->f1, e->n1));
      else if ((e->v < e->u) && (e->v < e->f2))
	hull.push_back(hullFace(e->v, e->u, e->f2, e->n2));
    }
    return hull; // Good hull
  }
private:
  struct hullEdge {
    int u, v, f1, f2; Vector n1, n2;
    hullEdge(int U, int V) : u(U), v(V), f1(-1), f2(-1) {}
  };
  list<hullEdge> edges; vector<int> halfE;
  void setF1(hullEdge &e,int f1) { e.f1=f1, e.n1=tNorm(pts[e.u],pts[e.v],pts[e.f1]); }
  void setF2(hullEdge &e,int f2) { e.f2=f2, e.n2=tNorm(pts[e.v],pts[e.u],pts[e.f2]); }
  void addPt(int i) {
    for (list<hullEdge>::iterator e = edges.begin(); e != edges.end(); ++e) {
      bool v1 = dot(pts[i] - pts[e->u], e->n1) > EPS;
      bool v2 = dot(pts[i] - pts[e->u], e->n2) > EPS;
      if(v1 && v2) e = --edges.erase(e);
      else if(v1) setF1(*e, i), addCone(e->u, e->v, i);
      else if(v2) setF2(*e, i), addCone(e->v, e->u, i);
    }
  }
  void addCone(int u, int v, int apex) {
    if (halfE[v] != -1){
      edges.push_front(hullEdge(v, apex));
      setF1(edges.front(), u), setF2(edges.front(), halfE[v]);
      halfE[v] = -1;
    } else halfE[v] = u;
    if (halfE[u] != -1){
      edges.push_front(hullEdge(apex, u));
      setF1(edges.front(), v); setF2(edges.front(), halfE[u]);
      halfE[u] = -1;
    } else halfE[u] = v;
  }
};

// Compute the volume of a convex polyhedron (input is an array of triangular faces)
typedef tuple<Vector,Vector,Vector> tvvv;
double volume_polyhedron(vector<tvvv>& p){
  Vector c,p0,p1,p2; double v, volume = 0;
  for(int i=0;i<p.size();i++)
    c = c + get<0>(p[i]) + get<1>(p[i]) + get<2>(p[i]);
  c = 1/(3.0*p.size())*c;
  for(int i=0;i<p.size();i++){
    tie(p0,p1,p2) = p[i], v = dot(p0,cross(p1,p2)) / 6;
    if(dot(cross(p2-p1,p0-p1),c-p0) > 0) volume -= v;
    else volume += v;
  }
  return volume;
}
