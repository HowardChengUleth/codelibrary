/*  Include SCC code
 *  O(n) solve 2SAT problem:
 *  solve().fst = T/F if there is a valid assignment
 *  solve().snd = vector<bool> containing the valid assignments.
 */
int VAR(int i) { return 2*i; }
int NOT(int i) { return i^1; }
struct SAT {
  int n; SCC scc;
  SAT(int n) : n(n), scc(2*n) {}
  void add_or(int a, int b) {
    if (a == NOT(b)) return;
    scc.add_edge(NOT(a), b);
    scc.add_edge(NOT(b), a);
  }
  void add_true(int a) { add_or(a, a); }
  void add_false(int a) { add_or(NOT(a), NOT(a)); }
  void add_xor(int a, int b) { add_or(a, b); add_or(NOT(a), NOT(b)); }
  pair<bool, vector<bool>> solve() {
    auto comp = scc.scc(); vector<bool> ans(n);
    for (int i = 0; i < 2*n; i += 2) {
      if (comp[i] == comp[i+1]) return {false, {}};
      ans[i/2] = (comp[i] > comp[i+1]);
    }
    return {true, ans};
  }
};

