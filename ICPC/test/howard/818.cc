#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;

typedef pair<int,int> pii;

struct UnionFind
{
  vector<int> uf;
  UnionFind(int n) : uf(n) {
    for (int i = 0; i < n; i++) uf[i] = i;
  }
  
  int find(int x) {
    return (uf[x] == x) ? x : (uf[x] = find(uf[x]));
  }
  
  bool merge(int x, int y) {
    int res1 = find(x), res2 = find(y);
    if (res1 == res2) return false;
    uf[res2] = res1;
    return true;
  }
};

bool member(int m, int i)
{
  return m & (1 << i);
}

bool check(int n, int m, const vector<pii> &edge)
{
  UnionFind uf(n);
  int deg[15] = {0};
  for (unsigned int i = 0; i < edge.size(); i++) {
    int a = edge[i].first, b = edge[i].second;
    if (member(m, a) || member(m, b)) continue;
    if (!uf.merge(a, b)) return false;                // cycle
    if (deg[a] > 1 || deg[b] > 1) return false;     // not a line
    deg[a]++;  deg[b]++;
  }

  bool label[15] = {false};
  int removed = 0;
  for (int i = 0; i < n; i++) {
    if (member(m, i)) {
      removed++;
    } else {
      label[uf.find(i)] = true;
    }
  }
  return count(label, label+n, true) - 1 <= removed;
}

#include <cassert>

bool solve(int id)
{
  int n;
  cin >> n;
  if (!n) return false;

  vector<pii> edge;
  set<pii> S;

  int i, j;
  while (cin >> i >> j && (i != -1 || j != -1)) {
    if (i > j) swap(i, j);
    pii p(i-1, j-1);
    if (!S.count(p)) {
      edge.push_back(p);
      S.insert(p);
    }
  }

  int best = n;
  for (int m = 0; m < (1 << n); m++) {
    if (check(n, m, edge)) {
      best = min(best, __builtin_popcount(m));
    }
  }

  cout << "Set " << id << ": Minimum links to open is " << best << endl;
  return true;
}

int main()
{
  int id = 1;
  while (solve(id++))
    ;
  return 0;
}
