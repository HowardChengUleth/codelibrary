typedef tuple<double,int,int> seg;

// (x1,y1) , (x2,y2) are corners of axis-aligned rectangles
struct rectangle{ double x1,y1,x2,y2; };

struct segment_tree{
  int n; const vector<double>& v;  vector<int> pop;  vector<double> len;
  segment_tree(const vector<double>& y) : n(y.size()),v(y),pop(2*n-3),len(2*n-3) {}

  double add(pair<double,double> s,int a){ return add(s,a,0,n-2); }
  double add(const pair<double,double>& s, int a, int lo, int hi){
    int m = (lo+hi)/2 + (lo == hi ? n-2 : 0);
    if(a && (v[lo] < s.second) && (s.first < v[hi+1])){
      if((s.first <= v[lo]) && (v[hi+1] <= s.second)){
	pop[m] += a;
	len[m] = (lo == hi ? 0 : add(s,0,lo,m) + add(s,0,m+1,hi));
      } else len[m] = add(s,a,lo,m) + add(s,a,m+1,hi);
      if(pop[m] > 0) len[m] = v[hi+1] - v[lo];
    }
    return len[m];
  }
};

double area_union_rectangles(vector<rectangle>& R){
  vector<double> y; vector<seg> v;
  for(int i=0;i<R.size();i++){
    if(R[i].x1 == R[i].x2 || R[i].y1 == R[i].y2) continue;
    y.push_back(R[i].y1), y.push_back(R[i].y2);
    if(R[i].y1 > R[i].y2) swap(R[i].y1,R[i].y2);
    v.push_back(seg(min(R[i].x1,R[i].x2),i, 1));
    v.push_back(seg(max(R[i].x1,R[i].x2),i,-1));
  }
  sort(v.begin(),v.end());  sort(y.begin(),y.end());
  y.resize(unique(y.begin(),y.end()) - y.begin());
  segment_tree s(y); double area = 0, amt = 0, last = 0;
  for(int i=0;i<v.size();i++){
    area += amt * (get<0>(v[i]) - last);
    last = get<0>(v[i]); int t = get<1>(v[i]);
    amt = s.add(make_pair(R[t].y1,R[t].y2),get<2>(v[i]));
  }
  return area;
}

