// CHT from monash code binder
// add lines of the form y = ax + b
// query maximum value at point x
template<typename T> struct DynamicHull {
  struct Line {
    typedef typename multiset<Line>::iterator It;
    T a, b; mutable It me, endit, none;
    Line(T a, T b, It endit) : a(a), b(b), endit(endit) {}
    bool operator<(const Line& rhs) const {
      if (rhs.endit != none) return a < rhs.a;
      if (next(me) == endit) return 0;
      return (b - next(me)->b) < (next(me)->a - a) * rhs.a;
    }
  };
  multiset<Line> lines;
  void add(T a, T b) {
    auto bad = [&](auto y) {
      auto z = next(y);
      if (y == lines.begin()) {
        if (z == lines.end()) return false;
        return y->a == z->a and z->b >= y->b;
      }
      auto x = prev(y);
      if (z == lines.end()) return y->a == x->a and x->b >= y->b;
      return (x->b-y->b) * (z->a-y->a) >= (y->b-z->b) * (y->a-x->a);
    };
    auto it = lines.emplace(a, b, lines.end()); it->me = it;
    if (bad(it)) { lines.erase(it); return; }
    while (next(it) != lines.end() and bad(next(it))) lines.erase(next(it));
    while (it != lines.begin() and bad(prev(it))) lines.erase(prev(it));
  }
  T query(T x) {
    auto it = lines.lower_bound(Line{x, 0, {}});
    return it->a * x + it->b;
  }
};
