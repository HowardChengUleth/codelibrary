#include "everything"
#include "../../geometry2D.cc"

int main(void)
{
  Point polygon[100000];
  int n = 0;
  while(cin >> polygon[n].x >> polygon[n].y)
    n++;
  cout << "Area = " << fixed << setprecision(2) 
       << area_polygon(polygon, n) << endl;
  return 0;
}
