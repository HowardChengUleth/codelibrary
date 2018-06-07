/*
 * Area of a polygon
 *
 * Author: Howard Cheng
 * Reference:
 *   http://www.exaflop.org/docs/cgafaq/cga2.html
 *
 * This routine returns the SIGNED area of a polygon represented as an
 * array of n points (n >= 1).  The result is positive if the orientation is
 * counterclockwise, and negative otherwise.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct {
  double x, y;
} Point;

double area_polygon(Point *polygon, int n)
{
  double sum = 0.0;
  int i;

  for (i = 0; i < n-1; i++) {
    sum += polygon[i].x * polygon[i+1].y - polygon[i].y * polygon[i+1].x;
  }
  sum += polygon[n-1].x * polygon[0].y - polygon[n-1].y * polygon[0].x;
  return sum/2.0;
}

int main(void)
{
  Point *polygon;
  int n;
  int i;

  while (scanf("%d", &n) == 1 && n > 0) {
    polygon = (Point *)malloc(n * sizeof(Point));
    assert(polygon);
    for (i = 0; i < n; i++) {
      scanf("%lf %lf", &polygon[i].x, &polygon[i].y);
    }
    printf("Area = %6.2f\n", area_polygon(polygon, n));
    free(polygon);
  }
  return 0;
}

