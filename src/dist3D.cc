//
// 3-D distances between point to point, point to line segment,
// line segment to line segment, and point to triangle.
//
// There are corresponding versions of the same code for distances
// between point to infinite lines, infinite line to infinite line,
// and point to infinite plane.
//
// It is assumed that segments/lines/triangles/plane are defined by
// distinct points (so the objects are not degenerate).
//
// They can be used for 2-D objects as well by setting the z coordinates
// to 0.
//
// Author: Howard Cheng
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

const double PI = acos(-1.0);

struct Vector {
  double x, y, z;

  Vector(double xx = 0, double yy = 0, double zz = 0)
    : x(xx), y(yy), z(zz) { }

  Vector(const Vector &p1, const Vector &p2)
    : x(p2.x - p1.x), y(p2.y - p1.y), z(p2.z - p1.z) { }

  Vector(const Vector &p1, const Vector &p2, double t)
    : x(p1.x + t*p2.x), y(p1.y + t*p2.y), z(p1.z + t*p2.z) { }
    
  double norm() const {
    return sqrt(x*x + y*y + z*z);
  }

};

istream &operator>>(istream &is, Vector &p)
{
  return is >> p.x >> p.y >> p.z;
}

ostream &operator<<(ostream &os, const Vector &p)
{
  return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

double dot(const Vector &p1, const Vector &p2)
{
  return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Vector cross(const Vector &p1, const Vector &p2)
{
  Vector v(p1.y*p2.z - p2.y*p1.z,
	   p2.x*p1.z - p1.x*p2.z,
	   p1.x*p2.y - p2.x*p1.y);
  return v;
}

// distance between two points
double dist_point_to_point(const Vector &p1, const Vector &p2)
{
  Vector p(p1, p2);
  return p.norm();
}

// angle between two vectors (in radians)
double angle(const Vector &p1, const Vector &p2)
{
  return acos(dot(p1, p2)/p1.norm()/p2.norm());
}

// distance from p to the line segment from a to b
double dist_point_to_segment(const Vector &p, const Vector &a,
			     const Vector &b)
{
  Vector u(a, p), v(a, b);
  double s = dot(u,v) / dot(v,v);
  
  if (s < 0 || s > 1) {
    return min(dist_point_to_point(p, a), dist_point_to_point(p, b));
  }

  Vector proj(a, v, s);
  return dist_point_to_point(proj, p);
}

// distance from p to the infinite line defined by a and b
double dist_point_to_line(const Vector &p, const Vector &a,
			  const Vector &b)
{
  Vector u(a, p), v(a, b);
  double s = dot(u,v) / dot(v,v);
  Vector proj(a, v, s);
  return dist_point_to_point(proj, p);
}

// distance from p to the triangle defined by a, b, c
double dist_point_to_triangle(const Vector &p, const Vector &a,
			      const Vector &b, const Vector &c)
{
  Vector u(a, p), v1(a, b), v2(a, c);
  Vector normal = cross(v1, v2);
  double s = dot(u, normal) / (normal.norm() * normal.norm());
  Vector proj(p, normal, -s);

  // check projection: inside if sum of angles is 2*pi
  Vector wa(proj, a), wb(proj, b), wc(proj, c);
  double a1 = angle(wa, wb);
  double a2 = angle(wa, wc);
  double a3 = angle(wb, wc);
  if (fabs(a1 + a2 + a3 - 2*PI) < 1e-8) {
    return dist_point_to_point(proj, p);
  } else {
    return min(dist_point_to_segment(p, a, b),
	       min(dist_point_to_segment(p, a, c),
		   dist_point_to_segment(p, b, c)));
  }
}

// distance from p to the infinite plane defined by a, b, c
double dist_point_to_plane(const Vector &p, const Vector &a,
			   const Vector &b, const Vector &c)
{
  Vector u(a, p), v1(a, b), v2(a, c);
  Vector normal = cross(v1, v2);
  double s = dot(u, normal) / (normal.norm() * normal.norm());
  Vector proj(p, normal, -s);
  return dist_point_to_point(proj, p);
}

// distance from segment p1->q1 to p2->q2
double dist_segment_to_segment(const Vector &p1, const Vector &q1,
			       const Vector &p2, const Vector &q2)
{
  //
  // the points on the 1st line are p1 + t * v1 
  // the points on the 2nd line are p2 + s * v2
  //
  //                             0 <= s, t <= 1
  //
  // squared distance is
  //
  // S = (p1.x - p2.x + t * v1.x - s * v2.x)^2 +
  //     (p1.y - p2.y + t * v1.y - s * v2.y)^2 +
  //     (p1.z - p2.z + t * v1.z - s * v2.z)^2
  //
  // deriviative wrt t and s are:
  //
  // 1/2 dS/dt = norm(v1)^2 * t - dot(v1, v2) * s + dot(v1, p1) - dot(v1, p2)
  // 1/2 dS/ds = -dot(v1, v2) * t + norm(v2)^2 * s - dot(v2, p1) + dot(v2, p2)
  //
  // solving for s and t with both derivatives = 0:
  //
  
  Vector v1(p1, q1), v2(p2, q2);
  Vector rhs(dot(v1, p2) - dot(v1, p1), dot(v2, p1) - dot(v2, p2));
  double det = v1.norm()*v1.norm()*v2.norm()*v2.norm() - 
    dot(v1, v2)*dot(v1, v2);

  if (det < 1e-8) {
    // parallel lines (if v1 and v2 != 0)
    goto degenerate;
  } else {
    double t = (rhs.x*v2.norm()*v2.norm() + rhs.y * dot(v1, v2)) / det;
    double s = (v1.norm()*v1.norm()*rhs.y + dot(v1, v2) * rhs.x) / det;
    if (0 <= s && s <= 1 && 0 <= t && t <= 1) {
      Vector pp1(p1, v1, t), pp2(p2, v2, s);
      return dist_point_to_point(pp1, pp2);
    }
  }

 degenerate:
  return min(min(dist_point_to_segment(p1, p2, q2),
		 dist_point_to_segment(q1, p2, q2)),
	     min(dist_point_to_segment(p2, p1, q1),
		 dist_point_to_segment(q2, p1, q1)));
}

// distance from infinite lines defined by p1->q1 and p2->q2
double dist_line_to_line(const Vector &p1, const Vector &q1,
			 const Vector &p2, const Vector &q2)
{
  //
  // the points on the 1st line are p1 + t * v1 
  // the points on the 2nd line are p2 + s * v2
  //
  //                             0 <= s, t <= 1
  //
  // squared distance is
  //
  // S = (p1.x - p2.x + t * v1.x - s * v2.x)^2 +
  //     (p1.y - p2.y + t * v1.y - s * v2.y)^2 +
  //     (p1.z - p2.z + t * v1.z - s * v2.z)^2
  //
  // deriviative wrt t and s are:
  //
  // 1/2 dS/dt = norm(v1)^2 * t - dot(v1, v2) * s + dot(v1, p1) - dot(v1, p2)
  // 1/2 dS/ds = -dot(v1, v2) * t + norm(v2)^2 * s - dot(v2, p1) + dot(v2, p2)
  //
  // solving for s and t with both derivatives = 0:
  //
  
  Vector v1(p1, q1), v2(p2, q2);
  Vector rhs(dot(v1, p2) - dot(v1, p1), dot(v2, p1) - dot(v2, p2));
  double det = v1.norm()*v1.norm()*v2.norm()*v2.norm() - 
    dot(v1, v2)*dot(v1, v2);

  if (det < 1e-8) {
    // parallel lines (if v1 and v2 != 0)
    return dist_point_to_line(p1, p2, q2);
  } else {
    double t = (rhs.x*v2.norm()*v2.norm() + rhs.y * dot(v1, v2)) / det;
    double s = (v1.norm()*v1.norm()*rhs.y + dot(v1, v2) * rhs.x) / det;
    Vector pp1(p1, v1, t), pp2(p2, v2, s);
    return dist_point_to_point(pp1, pp2);
  }
}

/////////////////////////////////////////////////////////////////////////
//
// This is the solution to 11836 (Star War)
//
/////////////////////////////////////////////////////////////////////////

void do_case()
{
  Vector t1[4], t2[4];
  for (int i = 0; i < 4; i++) {
    cin >> t1[i];
  }
  for (int i = 0; i < 4; i++) {
    cin >> t2[i];
  }

  double best = dist_point_to_point(t1[0], t2[0]);

  // vertex-face distance
  for (int i1 = 0; i1 < 4; i1++) {
    for (int j1 = 0; j1 < 4; j1++) {
      best = min(best, dist_point_to_triangle(t1[i1], t2[j1], t2[(j1+1)%4],
					      t2[(j1+2)%4]));
      best = min(best, dist_point_to_triangle(t2[i1], t1[j1], t1[(j1+1)%4],
					      t1[(j1+2)%4]));
    }
  }
  	
  // edge-edge distance
  for (int i1 = 0; i1 < 4; i1++) {
    for (int i2 = i1+1; i2 < 4; i2++) {
      for (int j1 = 0; j1 < 4; j1++) {
	for (int j2 = j1+1; j2 < 4; j2++) {
	  best = min(best, dist_segment_to_segment(t1[i1], t1[i2],
						   t2[j1], t2[j2]));
	}
      }
    }
  }

  cout << setprecision(2) << fixed << best << endl;
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
