/*
 * Parameters of circle from 3 points
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/
 *
 * This routine computes the parameters of a circle (center and radius)
 * from 3 points.  Returns non-zero if successful, zero if the three
 * points are colinear.
 *
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

using namespace std;

/* how close to call equal */
const double EPSILON = 1E-8;

struct Point {
  double x, y;
};

int circle(Point p1, Point p2, Point p3, Point &center, double &r)
{
  double a,b,c,d,e,f,g;

  a = p2.x - p1.x;
  b = p2.y - p1.y;
  c = p3.x - p1.x;
  d = p3.y - p1.y;
  e = a*(p1.x + p2.x) + b*(p1.y + p2.y);
  f = c*(p1.x + p3.x) + d*(p1.y + p3.y);
  g = 2.0*(a*(p3.y - p2.y) - b*(p3.x - p2.x));
  if (fabs(g) < EPSILON) {
    return 0;
  }
  center.x = (d*e - b*f) / g;
  center.y = (a*f - c*e) / g;
  r = sqrt((p1.x-center.x)*(p1.x-center.x) + (p1.y-center.y)*(p1.y-center.y));
  return 1;
}

int main(void)
{
  Point a, b, c, center;
  double r;

  while (cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y) {
    if (circle(a, b, c, center, r)) {
      cout << fixed << setprecision(3);
      cout << "center = (" << center.x << ", " << center.y << ")" << endl;
      cout << "radius = " << r << endl;
    } else {
      cout << "colinear" << endl;
    }
  }
  return 0;
}
