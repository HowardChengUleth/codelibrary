#include "everything"
#include "../../geometry2D.cc"

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
