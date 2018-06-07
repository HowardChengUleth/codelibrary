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
  int count, best_i, i;

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

int point_in_poly(Point *poly, int n, Point p)
{
  int i, j, c = 0;

  /* first check to see if point is one of the vertices */
  for (i = 0; i < n; i++) {
    if (fabs(p.x - poly[i].x) < EPSILON && fabs(p.y - poly[i].y) < EPSILON) {
      return 1;
    }
  }

  /* now check if it's on the boundary */
  for (i = 0; i < n-1; i++) {
    if (ccw(poly[i], poly[i+1], p) == CNEITHER) {
      return 1;
    }
  }
  if (ccw(poly[n-1], poly[0], p) == CNEITHER) {
    return 1;
  }

  /* finally check if it's inside */
  for (i = 0, j = n; i <= n; j = i++) {
    int i1 = (i == n) ? 0 : i;
    int j1 = (j == n) ? 0 : j;
    if (((poly[i1].y <= p.y && p.y < poly[j1].y) ||
         (poly[j1].y <= p.y && p.y < poly[i1].y)) &&
        (p.x < (poly[j1].x - poly[i1].x) * (p.y - poly[i1].y)
         / (poly[j1].y - poly[i1].y) + poly[i1].x))
      c = !c;
  }
  return c;
}

/* returns 1 if intersect at a point, 0 if not, -1 if the lines coincide */
int intersect_line(Point a, Point b, Point c, Point d, Point *p)
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
      p->x = a.x + r*(b.x - a.x);
      p->y = a.y + r*(b.y - a.y);
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

int intersect_polygon(Point *poly1, int n1, Point *poly2, int n2, Point **out)
{
  Point *newpoly, p;
  char *used;
  int new_n = n1 + n2 + n1*n2;
  int count, i, i2, j, j2, new_count;
  int n;

  newpoly = (Point *)malloc(new_n * sizeof(Point));
  *out = (Point *)malloc(new_n * sizeof(Point));
  used = malloc(new_n);
  assert(newpoly && *out && used);
  count = 0;
  for (i = 0; i < new_n; i++) {
    used[i] = 0;
  }
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
      if (intersect_line(poly1[i], poly1[i2], poly2[j], poly2[j2], &p) == 1) {
        newpoly[count++] = p;
      }
    }
  }

  if (count >= 3) {
    n = convex_hull(newpoly, count, *out);
    if (n < 3) {
      free(*out);
      n = 0;
    }
  } else {
    free(*out);
    n = 0;
  }

  /* eliminate duplicates */
  for (i = 0; i < n-1; i++) {
    for (j = i+1; j < n; j++) {
      if ((*out)[i].x == (*out)[j].x && (*out)[i].y == (*out)[j].y) {
        used[j] = 1;
      }
    }
  }
  j = 0;
  new_count = 0;
  for (i = 0; i < n; i++) {
    if (!used[i]) {
      (*out)[new_count++] = (*out)[i];
    }
  }
  n = new_count;

  free(used);
  free(newpoly);
  return n;
}

int read_poly(Point **poly)
{
  int n, i;

  scanf("%d", &n);
  if (n == 0) {
    return 0;
  }
  *poly = (Point *)malloc(n * sizeof(Point));
  assert(*poly);
  for (i = 0; i < n; i++) {
    scanf("%lf %lf", &((*poly)[i].x), &((*poly)[i].y));
  }
  return n;

}

int main(void)
{
  Point *poly1, *poly2, *intersection;
  int n1, n2, n3, i;

  while ((n1 = read_poly(&poly1))) {
    n2 = read_poly(&poly2);
    n3 = intersect_polygon(poly1, n1, poly2, n2, &intersection);
    free(poly1);
    free(poly2);
    if (n3 >= 3) {
      for (i = 0; i < n3; i++) {
        printf("(%0.2f, %0.2f) ", intersection[i].x, intersection[i].y);
      }
      printf("\n");
      free(intersection);
    } else {
      printf("Empty Intersection\n");
    }
  }

  return 0;
}
