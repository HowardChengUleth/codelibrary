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

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>

using namespace std;

struct Point {
  double x, y;
};

double area_polygon(Point polygon[], int n)
{
  double sum = 0.0;

  for (int i = 0; i < n-1; i++) {
    sum += polygon[i].x * polygon[i+1].y - polygon[i].y * polygon[i+1].x;
  }
  sum += polygon[n-1].x * polygon[0].y - polygon[n-1].y * polygon[0].x;
  return sum/2.0;
}

int main(void)
{
  Point *polygon;
  int n;

  while (cin >> n && n > 0) {
    polygon = new Point[n];
    assert(polygon);
    for (int i = 0; i < n; i++) {
      cin >> polygon[i].x >> polygon[i].y;
    }
    cout << "Area = " << fixed << setprecision(2) 
	 << area_polygon(polygon, n) << endl;
    delete[] polygon;
  }
  return 0;
}

