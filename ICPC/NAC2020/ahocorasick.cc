/*  Description: Aho-Corasick tree is used for multiple pattern matching.
 *  Initialize the tree with create(patterns). find(word) returns for each position
 *  the index of the longest word that ends there, or -1 if none. findAll(\_, word) finds all words
 *  (up to $N \sqrt N$ many if no duplicate patterns) that start at each position (shortest first).
 *  Duplicate patterns are allowed; empty patterns are not.
 *  To find the longest words that start at each position, reverse all input.
 *  Time: create is $O(26N)$ where $N$ is the sum of length of patterns.
 *  find is $O(M)$ where $M$ is the length of the word. findAll is $O(NM)$.
 */
typedef long long ll;
struct ahocorasick {
  enum { alpha = 26, first = 'a' };
  struct Node {
    // (nmatches is optional)
    int back, next[alpha], start = -1, end = -1, nmatches = 0;
    Node(int v) { memset(next, v, sizeof(next)); }
  };
  vector<Node> N; vector<int> backp;
  void insert(string& s, int j) {
    assert(!s.empty());
    int n = 0;
    for (auto& c : s) {
      int& m = N[n].next[c - first];
      if (m == -1) n = m = sz(N), N.emplace_back(-1);
      else n = m;
    }
    if (N[n].end == -1) N[n].start = j;
    backp.push_back(N[n].end);
    N[n].end = j;
    N[n].nmatches++;
  }
  ahocorasick(vector<string>& pat) {
    N.emplace_back(-1);
    for (int i = 0; i < sz(pat); i++) insert(pat[i], i);
    N[0].back = sz(N); N.emplace_back(0);
    queue<int> q;
    for (q.push(0); !q.empty(); q.pop()) {
      int n = q.front(), prev = N[n].back;
      for (int i = 0; i < alpha; i++) {
        int &ed = N[n].next[i], y = N[prev].next[i];
        if (ed == -1) ed = y;
        else {
          N[ed].back = y;
          (N[ed].end == -1 ? N[ed].end : backp[N[ed].start]) = N[y].end;
          N[ed].nmatches += N[y].nmatches;
          q.push(ed);
        }
      }
    }
  }
  pair<vector<int>, ll> find(string word) {
    int n = 0;
    vector<int> res; ll count = 0;
    for (auto& c : word) {
      n = N[n].next[c - first];
      res.push_back(N[n].end);
      count += N[n].nmatches;
    }
    return { res, count };
  }
  vector<vector<int>> findall(vector<string>& pat, string word) {
    vector<int> r = find(word).fst;
    vector<vector<int>> res(sz(word));
    for (int i = 0; i < sz(word); i++) {
      int ind = r[i];
      while (ind != -1) {
        res[i - sz(pat[ind]) + 1].push_back(ind);
        ind = backp[ind];
      }
    }
    return res;
  }
};

// example usage
vector<string> pat = { "a", "aa", "an", "na", "ana", "c", "cc", "ba", "ab" };
ahocorasick a(pat);

// count # of matches
debug(a.find("banana"));

// find all matches
auto ans = (a.findall(pat, "banana"));
for (int i = 0; i < sz(ans); i++) {
  debug(i);
  for (auto& match : ans[i]) {
    debug(pat[match]);
  }
}
