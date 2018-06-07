/*
 * Convex hull
 *
 * Author: Howard Cheng
 * Reference:
 *   http://wilma.cs.brown.edu/courses/cs016/packet/node25.html
 *
 * Given a list of n (n >= 1) points in an array, it returns the vertices of
 * the convex hull in counterclockwise order.  Also returns the number of
 * vertices in the convex hull.  Assumes that the hull array has been
 * allocated to store the right number of elements (n elements is safe).
 * The points in the original polygon will be re-ordered.
 *
 * Note: The hull contains a maximum number of points.  ie. all colinear
 *       points and non-distinct points are included in the hull.
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

/* Global point for computing convex hull */
Point start_p;

/* counterclockwise, clockwise, or undefined */
enum {CCW, CW, CNEITHER};

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

int cmp(const void *a, const void *b)
{
  Point *p1 = (Point *)a;
  Point *p2 = (Point *)b;
  if (p1->x < p2->x) {
    return -1;
  } else if (p1->x > p2->x) {
    return 1;
  } else if (p1->y < p2->y) {
    return -1;
  } else if (p1->y > p2->y) {
    return 1;
  } else {
    return 0;
  }
}

int ccw_cmp(const void *a, const void *b)
{
  Point *p1 = (Point *)a;
  Point *p2 = (Point *)b;
  if (ccw(start_p, *p1, *p2) == CCW) {
    return -1;
  } else if (ccw(start_p, *p2, *p1) == CCW) {
    return 1;
  } else {
    return 0;
  }
}

int convex_hull(Point *polygon, int n, Point *hull) {
  int count, best_i, i, j;

  qsort(polygon, n, sizeof(Point), cmp);
  for (i = n-1; i >= 1; i--) {
    if (fabs(polygon[i].x - polygon[i-1].x) < EPSILON &&
	fabs(polygon[i].y - polygon[i-1].y) < EPSILON) {
      for (j = i; j < n-1; j++) {
	polygon[j] = polygon[j+1];
      }
      n--;
    }
  }

  if (n == 1) {
    hull[0] = polygon[0];
    return 1;
  }

  /* find the first point: min y, and then min x */
  start_p.x = polygon[0].x;
  start_p.y = polygon[0].y;
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
  qsort((polygon+1), n-1, sizeof(Point), ccw_cmp);

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

int main(void)
{
  Point *polygon, *hull;
  int n, hull_size;
  int i;

  while (scanf("%d", &n) == 1 && n > 0) {
    polygon = (Point *)malloc(n * sizeof(Point));
    hull = (Point *)malloc(n * sizeof(Point));
    assert(polygon && hull);
    for (i = 0; i < n; i++) {
      scanf("%lf %lf", &polygon[i].x, &polygon[i].y);
    }
    hull_size = convex_hull(polygon, n, hull);
    printf("Sorted:\n");
    for (i = 0; i < n; i++) {
      printf("(%4.2f, %4.2f) ", polygon[i].x, polygon[i].y);
    }
    printf("\n");
    printf("Hull size = %d\n", hull_size);
    for (i = 0; i < hull_size; i++) {
      printf("(%4.2f, %4.2f) ", hull[i].x, hull[i].y);
    }
    printf("\n");
    free(polygon);
    free(hull);
  }
  return 0;
}
