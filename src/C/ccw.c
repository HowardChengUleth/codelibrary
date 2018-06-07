/*
 * Orientation analysis
 *
 * Author: Howard Cheng
 * Reference:
 *   http://wilma.cs.brown.edu/courses/cs016/packet/node18.html
 *
 * Given three points a, b, c, it returns whether the path from a to b to c
 * is counterclockwise, clockwise, or undefined.
 *
 * Undefined is returned if the 3 points are colinear, and c is between
 * a and b.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* how close to call equal */
#define EPSILON 1E-8

typedef struct {
  double x, y;
} Point;

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

int main(void)
{
  Point a, b, c;
  int a1, a2, a3, a4, a5, a6;
  int res;

  while (scanf("%d %d %d %d %d %d", &a1, &a2, &a3, &a4, &a5, &a6) == 6) {
    a.x = a1; a.y = a2;
    b.x = a3; b.y = a4;
    c.x = a5; c.y = a6;
    res = ccw(a,b,c);
    if (res == CW) {
      printf("CW\n");
    } else if (res == CCW) {
      printf("CCW\n");
    } else if (res == CNEITHER) {
      printf("CNEITHER\n");
    } else {
      printf("Help, I am in trouble!\n");
      exit(1);
    }
  }
  return 0;
}
