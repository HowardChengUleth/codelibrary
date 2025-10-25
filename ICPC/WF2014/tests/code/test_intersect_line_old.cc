/*
 * 2-D Line Intersection
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/cga1.html
 *
 * This routine takes two line segments specified by endpoints, and
 * determines whether they intersect at one point, infinitely points,
 * or no points.  In the first case, the point of intersection is also
 * returned.  The endpoints of a line must be different (otherwise,
 * the line is not defined).
 *
 */

#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

/* how close to call equal */
const double EPSILON = 1E-8;

struct Point {
  double x, y;
};

/* returns 1 if intersect at a point, 0 if not, -1 if the lines coincide */
int intersect_line(Point a, Point b, Point c, Point d, Point &p)
{
  Point t;
  double r, s;
  double denom, num1, num2;

  assert((a.x != b.x || a.y != b.y) && (c.x != d.x || c.y != d.y));

  num1 = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
  num2 = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
  denom = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);

  if (fabs(denom) >= EPSILON) {
    r = num1 / denom;
    s = num2 / denom;
    if (0-EPSILON <= r && r <= 1+EPSILON && 
        0-EPSILON <= s && s <= 1+EPSILON) {
      /* always do this part if we are interested in lines instead */
      /* of line segments                                          */
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
      /* I am not using "fuzzy comparisons" here, because the comparisons */
      /* are based on the input, not some derived quantities.  You may    */
      /* want to change that if the input points are computed somehow.    */

      /* two lines are the "same".  See if they overlap */
      if (a.x > b.x || (a.x == b.x && a.y > b.y)) {
        t = a;
        a = b;
        b = t;
      }
      if (c.x > d.x || (c.x == d.x && c.y > d.y)) {
        t = c;
        c = d;
        d = t;
      }
      if (a.x == b.x) {
        /* vertical lines */
        if (b.y == c.y) {
          p = b;
          return 1;
        } else if (a.y == d.y) {
          p = a;
          return 1;
        } else if (b.y < c.y || d.y < a.y) {
          return 0;
        } else {
          return -1;
        }
      } else {
        if (b.x == c.x) {
          p = b;
          return 1;
        } else if (a.x == d.x) {
          p = a;
          return 1;
        } else if (b.x < c.x || d.x < a.x) {
          return 0;
        } else {
          return -1;
        }
      }

      return -1;
    }
  }
}

int main(void)
{
  Point a, b, c, d, p;
  int res;

  while (cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y) {
    res = intersect_line(a, b, c, d, p);
    if (res == 1) {
      cout << "Intersect at (" << p.x << ", " << p.y << ")" << endl;
    } else if (res == 0) {
      cout << "Don't intersect" << endl;
    } else {
      cout << "Infinite number of intersections" << endl;
    }
  }

  return 0;
}
