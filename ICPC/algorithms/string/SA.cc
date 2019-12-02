struct SA {
  int n; string str; vector<int> sarray, lcp;
  suffix_array(string s) : n(s.size()), str(move(s)) { build_sarray(); build_lcp(); }
  void bucket(vector<int>& a, vector<int>& b, vector<int>& r, int n, int K, int off=0) {
    vector<int> c(K+1, 0);
    for (int i = 0; i < n; i++) c[r[a[i]+off]]++;
    for (int i = 0, sum = 0; i <= K; i++) { int t = c[i]; c[i] = sum; sum += t; }
    for (int i = 0; i < n; i++) b[c[r[a[i]+off]]++] = a[i];
  }
  void build_sarray() {
    sarray.assign(n, 0); vector<int> r(2*n, 0), sa(2*n), tmp(2*n); if (n <= 1) return;
    for (int i = 0; i < n; i++) r[i] = (int) str[i] - CHAR_MIN + 1, sa[i] = i;
    for (int k = 1; k < n; k *= 2) {
      bucket(sa, tmp, r, n, max(n, 256), k), bucket(tmp, sa, r, n, max(n, 256), 0);
      tmp[sa[0]] = 1;
      for (int i = 1; i < n; i++) {
        tmp[sa[i]] = tmp[sa[i-1]];
        if ((r[sa[i]] != r[sa[i-1]]) || (r[sa[i]+k] != r[sa[i-1]+k])) tmp[sa[i]]++;
      }
      copy(tmp.begin(), tmp.begin()+n, r.begin());
    }
    copy(sa.begin(), sa.begin()+n, sarray.begin());
  }
  void build_lcp() {
    int h = 0; vector<int> rank(n); lcp.assign(n, 0);
    for (int i = 0; i < n; i++) rank[sarray[i]] = i;
    for (int i = 0; i < n; i++) {
      if (rank[i] > 0) {
        int j = sarray[rank[i] - 1];
        while (i + h < n and j + h < n and str[i+h] == str[j+h]) h++;
        lcp[rank[i]] = h;
      }
      if (h > 0) h--;
    }
  }
};

