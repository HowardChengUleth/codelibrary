#include "everything"
#include "../../geometry2D.cc"

int main(){
  Line L;
  Circle C;
  Point a1,a2;
  while(cin >> L.a.x >> L.a.y >> L.b.x >> L.b.y
	>> C.c.x >> C.c.y >> C.r){
    int num = intersect_iline_circle(L,C,a1,a2);
    if(num == 0)
      cout << "NO INTERSECTION." << endl;
    else if(num == 1)
      cout << "ONE INTERSECTION:  (" << a1.x << "," << a1.y << ")" << endl;
    else
      cout << "TWO INTERSECTIONS: (" << a1.x << "," << a1.y << ")" 
	   << "(" << a2.x << "," << a2.y << ")" << endl;
  }
  return 0;
}
