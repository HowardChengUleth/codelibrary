#include "everything"
#include "../../geometry2D.cc"

int main(void)
{
  Point a, b, c, d, p;
  int res;

  while (cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y) {
    res = intersect_line(a, b, c, d, p, true);
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
