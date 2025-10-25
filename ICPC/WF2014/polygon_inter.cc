/*
 * Convex Polygon Intersection
 *
 * Author: Howard Cheng
 *
 * This routine takes two convex polygon, and returns the intersection
 * which is also convex.  If the intersection contains less than
 * 3 points, it is considered empty.
 *
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cassert>

using namespace std;

/* how close to call equal */
const double EPSILON = 1E-8;

struct Point {
  double x, y;
};

const bool BOUNDARY = true;

/* counterclockwise, clockwise, or undefined */
enum Orientation {CCW, CW, CNEITHER};

/* Global point for computing convex hull */
Point start_p;

Orientation ccw(Point a, Point b, Point c)
{
  double dx1 = b.x - a.x;
  double dx2 = c.x - b.x;
  double dy1 = b.y - a.y;
  double dy2 = c.y - b.y;
  double t1 = dy2 * dx1;
  double t2 = dy1 * dx2;

  if (fabs(t1 - t2) < EPSILON) {
    if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
      if (dx1*dx1 + dy1*dy1 >= dx2*dx2 + dy2*dy2 - EPSILON) {
        return CNEITHER;
      } else {
        return CW;
      }
    } else {
      return CCW;
    }
  } else if (t1 > t2) {
    return CCW;
  } else {
    return CW;
  }
}

bool ccw_cmp(const Point &a, const Point &b)
{
  return ccw(start_p, a, b) == CCW;
}

int convex_hull(Point polygon[], int n, Point hull[]) {
  int count, best_i, i;

  if (n == 1) {
    hull[0] = polygon[0];
    return 1;
  }

  /* find the first point: min y, and then min x */
  start_p = polygon[0];
  best_i = 0;
  for (i = 1; i < n; i++) {
    if ((polygon[i].y < start_p.y) ||
        (polygon[i].y == start_p.y && polygon[i].x < start_p.x)) {
      start_p = polygon[i];
      best_i = i;
    }
  }
  polygon[best_i] = polygon[0];
  polygon[0] = start_p;

  /* get simple closed polygon */
  sort(polygon+1, polygon+n, ccw_cmp);

  /* do convex hull */
  count = 0;
  hull[count] = polygon[count]; count++;
  hull[count] = polygon[count]; count++;
  for (i = 2; i < n; i++) {
    while (count > 1 &&
           ccw(hull[count-2], hull[count-1], polygon[i]) == CW) {
      /* pop point */
      count--;
    }
    hull[count++] = polygon[i];
  }
  return count;
}

bool point_in_poly(Point poly[], int n, Point p)
{
  int i, j, c = 0;

  /* first check to see if point is one of the vertices */
  for (i = 0; i < n; i++) {
    if (fabs(p.x - poly[i].x) < EPSILON && fabs(p.y - poly[i].y) < EPSILON) {
      return BOUNDARY;
    }
  }

  /* now check if it's on the boundary */
  for (i = 0; i < n-1; i++) {
    if (ccw(poly[i], poly[i+1], p) == CNEITHER) {
      return BOUNDARY;
    }
  }
  if (ccw(poly[n-1], poly[0], p) == CNEITHER) {
    return BOUNDARY;
  }

  /* finally check if it's inside */
  for (i = 0, j = n-1; i < n; j = i++) {
    if (((poly[i].y <= p.y && p.y < poly[j].y) ||
        (poly[j].y <= p.y && p.y < poly[i].y)) &&
        (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y)
         / (poly[j].y - poly[i].y) + poly[i].x))
      c = !c;
  }
  return c;
}

/* returns 1 if intersect at a point, 0 if not, -1 if the lines coincide */
int intersect_line(Point a, Point b, Point c, Point d, Point &p)
{
  double r, s;
  double denom, num1, num2;
 
  num1 = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
  num2 = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
  denom = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);
 
  if (fabs(denom) >= EPSILON) {
    r = num1 / denom;
    s = num2 / denom;
    if (-EPSILON <= r && r <= 1+EPSILON && -EPSILON <= s && s <= 1+EPSILON) {
      p.x = a.x + r*(b.x - a.x);
      p.y = a.y + r*(b.y - a.y);
      return 1;
    } else {
      return 0;
    }
  } else {
    if (fabs(num1) >= EPSILON) {
      return 0;
    } else {
      return -1;
    }
  }
}

int intersect_polygon(Point poly1[], int n1, Point poly2[], int n2, 
		      Point *&out)
{
  Point *newpoly, p;
  char *used;
  int new_n = n1 + n2 + n1*n2;
  int count, i, i2, j, j2, new_count;
  int n;

  newpoly = new Point[new_n];
  out = new Point[new_n];
  used = new char[new_n];
  assert(newpoly && out && used);
  count = 0;
  fill(used, used+new_n, 0);

  for (i = 0; i < n1; i++) {
    if (point_in_poly(poly2, n2, poly1[i])) {
      newpoly[count++] = poly1[i];
    }
  }
  for (i = 0; i < n2; i++) {
    if (point_in_poly(poly1, n1, poly2[i])) {
      newpoly[count++] = poly2[i];
    }
  }

  for (i = 0; i < n1; i++) {
    i2 = (i+1 == n1) ? 0 : i+1;
    for (j = 0; j < n2; j++) {
      j2 = (j+1 == n2) ? 0 : j+1;
      if (intersect_line(poly1[i], poly1[i2], poly2[j], poly2[j2], p) == 1) {
        newpoly[count++] = p;
      }
    }
  }

  if (count >= 3) {
    n = convex_hull(newpoly, count, out);
    if (n < 3) {
      delete[] out;
      n = 0;
    }
  } else {
    delete[] out;
    n = 0;
  }

  /* eliminate duplicates */
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if (out[i].x == out[j].x && out[i].y == out[j].y) {
        used[j] = 1;
      }
    }
  }
  j = 0;
  new_count = 0;
  for (i = 0; i < n; i++) {
    if (!used[i]) {
      out[new_count++] = out[i];
    }
  }
  n = new_count;

  delete[] newpoly;
  delete[] used;
  return n;
}

int read_poly(Point *&poly)
{
  int n, i;

  cin >> n;
  if (n == 0) {
    return 0;
  }
  poly = new Point[n];
  assert(poly);
  for (i = 0; i < n; i++) {
    cin >> poly[i].x >> poly[i].y;
  }
  return n;

}

int main(void)
{
  Point *poly1, *poly2, *intersection;
  int n1, n2, n3, i;

  while ((n1 = read_poly(poly1))) {
    n2 = read_poly(poly2);
    n3 = intersect_polygon(poly1, n1, poly2, n2, intersection);
    delete[] poly1;
    delete[] poly2;
    if (n3 >= 3) {
      for (i = 0; i < n3; i++) {
	cout << fixed << setprecision(2);
	cout << "(" << intersection[i].x << ", " << intersection[i].y
	     << ") ";
      }
      cout << endl;
      delete[] intersection;
    } else {
      cout << "Empty Intersection" << endl;
    }
  }

  return 0;
}
