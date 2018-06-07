#include "everything"
#include "../../geometry2D.cc"

void print(const Point& a){
  cout << "(";
  if(fabs(a.x) < 1e-4)
    cout << "0.000";
  else
    cout << a.x;
  cout << ",";
  if(fabs(a.y) < 1e-4)
    cout << "0.000";
  else
    cout << a.y;
  cout << ")";
}

int main(){
  cout << fixed << setprecision(3);
  Circle C1,C2;
  Point a1,a2;
	
  while(cin >> C1.c.x >> C1.c.y >> C1.r >> C2.c.x >> C2.c.y >> C2.r){
    int num = intersect_circle_circle(C1,C2,a1,a2);
    if(a1.x < a2.x || (a1.x == a2.x && a1.y < a2.y)) swap(a1,a2);
    switch(num){
    case 0:
      cout << "NO INTERSECTION" << endl;
      break;
    case 1:
      print(a1); cout << endl;
      break;
    case 2:
      print(a1);print(a2);cout << endl;
      break;
    case 3:
      cout << "THE CIRCLES ARE THE SAME" << endl;
      break;
    }
  }
  return 0;
}
