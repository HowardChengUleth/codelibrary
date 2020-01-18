/*  Aho-Corasick
 *  match set of words against test
 *  O(m) build, O(n) match, O(n*k) list matches
 */
typedef long long ll;
struct ahocorasick {
  struct State { map<char, int> edge; int link, cnt, tot; vector<int> output; };
  int n, k; vector<State> node; vector<int> len;
  int make_node() { node.emplace_back(); return n++; }
  void add(const string& s) {
    int v = 0;
    for (char c : s) {
      if (!node[v].edge[c]) node[v].edge[c] = make_node();
      v = node[v].edge[c];
    }
    node[v].cnt++, node[v].output.push_back(k++), len.push_back(sz(s));
  }
  void build_links() {
    node[0].link = -1, node[0].tot = 0; queue<int> q; q.push(0);
    while (!q.empty()) {
      int v = q.front(); q.pop(); node[v].tot = node[v].cnt;
      if (node[v].link != -1) node[v].tot += node[node[v].link].tot;
      for (auto it : node[v].edge) {
        int c = it.fst, u = it.snd, j = node[v].link;
        while (j != -1 and !node[j].edge[c]) j = node[j].link;
        if (j != -1) node[u].link = node[j].edge[c];
        q.push(u);
      }
    }
  }
  ahocorasick() : n(1), k(0), node(1) {}
  ll count_matches(string x) { // O(sz(x)) find # matches in dictionary
    ll ans = 0; int v = 0;
    for (int i = 0; i < sz(x); i++) {
      while (v and !node[v].edge[x[i]]) v = node[v].link;
      v = node[v].edge[x[i]]; ans += node[v].tot;
    }
    return ans;
  }
};

