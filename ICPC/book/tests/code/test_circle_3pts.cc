#include "everything"
#include "../../geometry2D.cc"

int main(void)
{
  Point a, b, c, center;
  double r;

  while (cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y) {
    if (circle3pt(a, b, c, center, r)) {
      cout << fixed << setprecision(3);
      cout << "center = (" << center.x << ", " << center.y << ")" << endl;
      cout << "radius = " << r << endl;
    } else {
      cout << "colinear" << endl;
    }
  }
  return 0;
}
