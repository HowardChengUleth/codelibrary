//------------------------------------------------------------------------------
// 2D Integer geometry starts here

typedef long long ll;
bool dEqual(ll x, ll y) { return x == y; }  // replaces dEqual from double code
const ll EPS = 0;                           // replaces EPS from double code
struct Point {
  ll x, y;
  // safe ranges for x and y:
  // SR1 : -10^18<=x,y<=10^18,  SR2 : -10^9<=x,y<=10^9
  // SR3 : -10^6<=x,y<=10^6,    SR4 : -3*10^4<=x,y<=3*10^4
  // operator== and operator<: use double geometry code
};

// +, -, inv: SR1
//  *, cross: SR2
ll len2(const Point &p){ return p*p; } // len2=len*len // SR2

//------------------------------------------------------------------------------
// Colinearity test // SR2
// Orientation test // SR2
// Signed Area of Polygon (*2) // SR2 divided by n, don't divide by 2
//------------------------------------------------------------------------------
// Convex hull: 
//   To remove colinear pts: Change ("<0" and ">0") to ("<=0" and ">=0") // SR2
//------------------------------------------------------------------------------
// Point in Polygon Test // SR2

//------------------------------------------------------------------------------
// Squared distance from "c" to the infinite line defined by "a" and "b"
frac dist_line2(Point a, Point b, Point c) // SR4
{ ll cr=cross(b-a,a-c);return make_frac(cr*cr,len2(b-a)); }

//------------------------------------------------------------------------------
// Intersection of lines (line segment or infinite line) // SR3
//   (1 == 1 intersection pt, 0 == no intersection pts, -1 == infinitely many
int intersect_line(Point a, Point b, Point c, Point d, 
		   frac &px, frac &py,bool segment) {
  ll num1 = cross(d-c,a-c), num2 = cross(b-a,a-c),denom = cross(b-a,d-c);
  if (denom!=0) {
    if(!segment || (denom<0 && num1<=0 && num1>=denom && num2<=0 && num2>=denom) ||
       (denom>0 && num1>=0 && num1<=denom && num2>=0 && num2<=denom)) {
      px=make_frac(a.x,1)+make_frac(num1,denom)*make_frac((b-a).x,1);
      py=make_frac(a.y,1)+make_frac(num1,denom)*make_frac((b-a).y,1); return 1;
    } else return 0;
  }
  if(!segment) return (num1==0) ? -1 : 0; // For infinite lines, this is the end
  if (num1!=0) return 0;
  if(b < a) swap(a,b); if(d < c) swap(c,d);
  if (a.x == b.x) {
    if (b.y == c.y) { px=make_frac(b.x,1); py=make_frac(b.y,1); return 1; }
    if (a.y == d.y) { px=make_frac(a.x,1); py=make_frac(a.y,1); return 1; }
    return (b.y < c.y || d.y < a.y) ? 0 : -1;
  } else if (b.x == c.x) { px=make_frac(b.x,1); py=make_frac(b.y,1); return 1; }
  else if (a.x == d.x) { px=make_frac(a.x,1); py=make_frac(a.y,1); return 1; }
  else if (b.x < c.x || d.x < a.x) return 0;
  return -1;
}

//------------------------------------------------------------------------------
// Circle From 3 Points // SR3
bool circle3pt(Point a, Point b, Point c, // r2= r*r to avoid irrational numbers
	       frac &centerx, frac & centery, frac &r2) { 
  ll g = 2*cross((b-a),(c-b)); if (g==0) return false; // colinear points
  frac e= make_frac((b-a)*(b+a),g), f=make_frac((c-a)*(c+a),g);

  centerx= (f*make_frac((b-a).y,1) - e*make_frac((c-a).y,1)) * make_frac(-1 ,1); 
  centery= f*make_frac((b-a).x,1) - e*make_frac((c-a).x,1);

  frac tx=make_frac(a.x,1)-centerx,  ty=make_frac(a.y,1)-centery;
  r2=tx*tx+ty*ty;
  return true;
}
