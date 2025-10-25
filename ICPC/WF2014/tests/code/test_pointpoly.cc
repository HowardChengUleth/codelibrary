#include "everything"
#include "../../geometry2D.cc"

int main(void)
{
  Point polygon[100000], p;
  int i;

  int n = 0;
  cin >> p.x >> p.y;
  while(cin >> polygon[n].x >> polygon[n].y)
    n++;

  if (point_in_poly(polygon, n, p)) {
    cout << "yes";
  } else {
    cout << "no";
  }
  
  return 0;
}
