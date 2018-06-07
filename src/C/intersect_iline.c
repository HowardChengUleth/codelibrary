/*
 * 2-D Line Intersection
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/cga1.html
 *
 * This routine takes two infinite lines specified by two points, and
 * determines whether they intersect at one point, infinitely points,
 * or no points.  In the first case, the point of intersection is also
 * returned.  The points of a line must be different (otherwise,
 * the line is not defined).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* how close to call equal */
#define EPSILON 1E-8

typedef struct {
  double x, y;
} Point;

/* returns 1 if intersect at a point, 0 if not, -1 if the lines coincide */
int intersect_iline(Point a, Point b, Point c, Point d, Point *p)
{
  double r;
  double denom, num1, num2;

  assert((a.x != b.x || a.y != b.y) && (c.x != d.x || c.y != d.y));

  num1 = (a.y - c.y)*(d.x - c.x) - (a.x - c.x)*(d.y - c.y);
  num2 = (a.y - c.y)*(b.x - a.x) - (a.x - c.x)*(b.y - a.y);
  denom = (b.x - a.x)*(d.y - c.y) - (b.y - a.y)*(d.x - c.x);

  if (fabs(denom) >= EPSILON) {
    r = num1 / denom;
    p->x = a.x + r*(b.x - a.x);
    p->y = a.y + r*(b.y - a.y);
    return 1;
  } else {
    if (fabs(num1) >= EPSILON) {
      return 0;
    } else {
      return -1;
    }
  }
}

int main(void)
{
  Point a, b, c, d, p;
  int res;

  while (scanf("%lf %lf %lf %lf %lf %lf %lf %lf",
         &a.x, &a.y, &b.x, &b.y, &c.x, &c.y, &d.x, &d.y) == 8) {
    res = intersect_iline(a, b, c, d, &p);
    if (res == 1) {
      printf("Intersect at (%0.2f, %0.2f)\n", p.x, p.y);
    } else if (res == 0) {
      printf("Don't intersect\n");
    } else {
      printf("Infinite number of intersections\n");
    }
  }

  return 0;
}
