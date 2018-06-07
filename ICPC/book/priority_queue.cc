// Priority Queue (maximum) that only allows one of each type of handle in at one time
// For minimum, flip the inequalities between the d[]'s on the lines that have "////"
class PriorityQueue{
private: vector<int> q,d,loc;  int n;
public:
  PriorityQueue(int N) : q(N,-1),d(N),loc(N,-1),n(0) {} // N == number of handles
  int top()    const { return d[q[0]]; }                // May not give RTE if empty.
  bool empty() const { return n == 0; }                 // int size() { return n; }
  
  void push(int value, int h){ // h is your 'handle' ... assert(0 =< h && h < n);
    if(loc[h] != -1 && d[h] <= value) return;
    d[h] = value;    if(loc[h] == -1) { loc[q[n] = h] = n; n++; }
    for(int i=loc[h],j=(i-1)/2 ; i>0 && d[q[i]] > d[q[j]] ; i=j,j=(j-1)/2) ////
      swap(loc[q[i]],loc[q[j]]), swap(q[i],q[j]);
  }
  
  void pop(){ // Does not check for empty queue
    loc[q[0]] = -1; q[0] = q[--n];
    for(int i=0,j=1 ; j<n ; i=j,j=2*j+1){
      if(j+1 < n && d[q[j+1]] > d[q[j]]) j++; ////
      if(d[q[j]] <= d[q[i]]) break;           ////
      swap(loc[q[i]],loc[q[j]]), swap(q[i],q[j]);
    }
  }
};
