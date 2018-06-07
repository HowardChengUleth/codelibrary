#include "everything"
#include "../../geometry2D.cc"

int main(void)
{
  Point polygon[100000], hull[100000];
  int hull_size;
  int i;

  int n = 0;
  while(cin >> polygon[n].x >> polygon[n].y)
    n++;
  hull_size = convex_hull(polygon, n, hull);
  cout << "Sorted:" << endl;
  for (i = 0; i < n; i++) {
    cout << fixed << setprecision(2);
    cout << "(" << polygon[i].x << ", " << polygon[i].y << ")" << endl;
  }
  cout << endl;
  cout << "Hull size = " << hull_size << endl;
  for (i = 0; i < hull_size; i++) {
    cout << "(" << hull[i].x << ", " << hull[i].y << ")" << endl;
  }
  cout << endl;
  return 0;
}
