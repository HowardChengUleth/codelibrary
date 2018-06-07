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

#include <iostream>
#include <cmath>

using namespace std;

/* how close to call equal */
const double EPSILON = 1E-8;

struct Point {
  double x, y;
};

/* counterclockwise, clockwise, or undefined */
enum Orientation {CCW, CW, CNEITHER};

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

int main(void)
{
  Point a, b, c;
  Orientation res;

  while (cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y) {
    res = ccw(a,b,c);
    if (res == CW) {
      cout << "CW" << endl;
    } else if (res == CCW) {
      cout << "CCW" << endl;
    } else if (res == CNEITHER) {
      cout << "CNEITHER" << endl;
    } else {
      printf("Help, I am in trouble!\n");
      exit(1);
    }
  }
  return 0;
}
