// MinQueue: maintain a standard queue while being able to query the min element
//  Constant time (amortized) per push/pop operation can be changed to maintain
//  max (or both min/max). No checks for empty queues anywhere!

class MinQueue {
private:
  stack<pair<int,int> > s1;   stack<int> s2;   int m1, m2;

  void move() {
    if (!s1.empty()) return;
    while (!s2.empty()) {
      s1.push(make_pair(s2.top(), m1));
      m1 = ::min(s2.top(), m1);
      s2.pop();
    }
    m2 = INT_MAX;         // min of empty queue
  }
  
public:
  // whatever the min of an empty queue should be
  MinQueue() : m1(INT_MAX), m2(INT_MAX) { }

  int min()    const { return ::min(m1, m2); }
  bool empty() const { return s1.empty() && s2.empty();  }
  void push(int x)   { m2 = ::min(m2, x); s2.push(x); }
  int front()        { move(); return s1.top().first; }
  void pop()         { move(); m1 = s1.top().second; s1.pop(); }
};
