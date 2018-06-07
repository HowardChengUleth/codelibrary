/*
 * Distance from a point to a line.
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/cga1.html
 *
 * This routine computes the shortest distance from a point to a line.
 * ie. distance from point to its orthogonal projection onto the line.
 * Works even if the projection is not on the line.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct {
  double x, y;
} Point;

/* computes the distance from "c" to the line defined by "a" and "b" */
double dist_line(Point a, Point b, Point c)
{
  double L2, s;

  L2 = (b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y);
  assert(L2 > 0);
  s = ((a.y-c.y)*(b.x-a.x)-(a.x-c.x)*(b.y-a.y)) / L2;

  return fabs(s*sqrt(L2));
}

int main(void)
{
  Point a, b, c;

  while (scanf("%lf %lf %lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y, &c.x, &c.y)
         == 6) {
    printf("distance = %6.2f\n", dist_line(a, b, c));
  }
  return 0;
}
