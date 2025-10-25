// Given a set of linear lines (non-vertical of the form y=mx+b):
//   gives you the minimum value at any given x-value.
// Look at lines with comments like this //// for max_value

typedef long long ll;
typedef long double ld;

const ll oo = LONG_LONG_MAX; // Infinity
bool XXX = true;
struct Line{
  ld m,b; bool d;       mutable ld xL,xR; 
  Line(ld x_)             : m(0) , b(0) , d(false), xL(x_), xR(x_) { }
  Line(ld m_,ld b_)       : m(m_), b(b_), d(false), xL(0) , xR(0)  { }
  Line(ld x_,ld m_,ld b_) : m(m_), b(b_), d(true) , xL(x_), xR(x_) { }
  
  bool operator<(const Line& L) const {
    if(XXX) return (xR != L.xR ? xR > L.xR : xL > L.xL);
    return (m != L.m ? m < L.m : b < L.b);
  }
};

bool cw(const Line& L1,const Line& L2,const Line& L3){
  complex<ld> a(L1.m,L1.b), b(L2.m,L2.b), c(L3.m,L3.b);
  return (L1.d || L2.d || L3.d || imag(conj(b-a)*(c-b)) < 0);
}

ld intersect(const Line& L1,const Line& L2){ return (L1.b-L2.b) / (L1.m-L2.m); }

// Completely general (no assumptions)
struct Hull{ // add_line = O(log n)   min_value = O(log n)  
  set<Line> H; set<Line>::iterator it,itL,itR;
  Hull() { H.insert(Line(oo,-oo,-oo)); H.insert(Line(-oo,oo,-oo)); }

  void add_line(ld m,ld b){
    Line L(m,-b);    // For max_value: Line L(-m,b);
    if(H.size() == 2){ L.xL = oo, L.xR = -oo;  H.insert(L); return; }
    XXX = 0; itL = itR = H.upper_bound(L); --itL;
    
    if((L.m == itL->m && L.b <= itL->b) || (L.m == itR->m && L.b <= itR->b)) return;
    if(!cw(*itL,L,*itR)) return;
        
    if(!itL->d) for(it=itL--;(it->m==L.m || !cw(*itL,*it,L));it=itL--) H.erase(it);
    if(!itR->d) for(it=itR++;(it->m==L.m || !cw(L,*it,*itR));it=itR++) H.erase(it);
    it = itL = itR = H.insert(itR,L);  --itL; ++itR;
    
    it->xL = itL->xR = (itL->d ?  oo : intersect(*it,*itL));
    it->xR = itR->xL = (itR->d ? -oo : intersect(*it,*itR));
  }
  
  ld min_value(ld x){
    XXX = 1;  it = H.lower_bound(x);
    return (it->m)*x - (it->b);     ////   return -(it->m)*x + (it->b);
  }

  ld min_value_inc_x(ld x){ // If you know every x is larger (or the same)
    itR = H.end(); advance(itR,-2);
    for(it=itR-- ; x > it->xL ; it=itR-- ) H.erase(it);
    it->xR = -oo;
    return (it->m)*x - (it->b);     ////   return -(it->m)*x + (it->b);
  }

  ld min_value_dec_x(ld x){ // If you know every x is smaller (or the same)
    itL = H.begin(); ++itL;
    for(it=itL++ ; x < it->xR ; it=itL++ ) H.erase(it);
    it->xL = oo;
    return (it->m)*x - (it->b);     ////   return -(it->m)*x + (it->b);
  }
};

// Assumes that slopes are non-increasing (or non-decreasing)
struct Hull2{ // add_line = O(1)    min_value = O(log n)
  deque<Line> H;

  void add_line_slope_inc(ld m,ld b){
    Line L(m,-b);                   //// Line L(-m,b);
    if(H.empty()) { L.xL = oo, L.xR = -oo; H.push_back(L); return; }
    if(H.back().m == L.m && H.back().b >= L.b) return;
    while(H.size() > 1 && !cw(H[H.size()-2],H.back(),L)) H.pop_back();
    L.xR = -oo; L.xL = H.back().xR = intersect(H.back(),L);
    H.push_back(L);
  }
  void add_line_slope_dec(ld m,ld b){
    Line L(m,-b);                   //// Line L(-m,b);
    if(H.empty()) { L.xL = oo, L.xR = -oo; H.push_front(L); return; }
    if(H[0].m == L.m && H[0].b >= L.b) return;
    while(H.size() > 1 && !cw(L,H[0],H[1])) H.pop_front();
    L.xR = -oo; L.xL = H[0].xR = intersect(H[0],L);
    H.push_front(L);
  }
  
  ld min_value(ld x){
    XXX = 1; int i = lower_bound(H.begin(),H.end(),x) - H.begin();
    return H[i].m*x - H[i].b;         ////   return -H[i].m*x + H[i].b;
  }

  ld min_value_inc_x(ld x){ // If you know every x is larger (or the same)
    while(H.back().xL < x) { H.pop_back(); } H.back().xR = -oo;
    return (H.back().m)*x - (H.back().b); //// return -(H.back().m)*x+(H.back().b);
  }

  ld min_value_dec_x(ld x){ // If you know every x is smaller (or the same)
    while(H[0].xR > x) { H.pop_front(); }  H[0].xL = oo;
    return (H[0].m)*x - (H[0].b); //// return -(H[0].m)*x + (H[0].b);
  }
};

