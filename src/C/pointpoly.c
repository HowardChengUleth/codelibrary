/*
 * Point-in-polygon test
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/cga2.html
 *
 * Given a polygon as a list of n vertices, and a point, it returns
 * whether the point is in the polygon or not.
 *
 * One has the option to define the behavior on the boundary.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* how close to call equal */
#define EPSILON 1E-8

/* what should be returned on the boundary? */
#define BOUNDARY 1

/* counterclockwise, clockwise, or undefined */
enum {CCW, CW, CNEITHER};

typedef struct {
  double x, y;
} Point;

int ccw(Point a, Point b, Point c)
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

int point_in_poly(Point *poly, int n, Point p)
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

int main(void)
{
  Point *polygon, p;
  int n;
  int i;

  while (scanf("%d", &n) == 1 && n > 0) {
    polygon = (Point *)malloc(n * sizeof(Point));
    assert(polygon);
    for (i = 0; i < n; i++) {
      scanf("%lf %lf", &polygon[i].x, &polygon[i].y);
    }
    while (scanf("%lf %lf", &p.x, &p.y) == 2) {
      if (point_in_poly(polygon, n, p)) {
        printf("yes\n");
      } else {
        printf("no\n");
      }
    }
    free(polygon);
  }
  return 0;
}
