// KMP
void prepare_pattern(const string &pat, vector<int> &T) {
  int n = pat.length();
  T.resize(n+1);
  fill(T.begin(), T.end(), -1);
  for (int i = 1; i <= n; i++) {
    int pos = T[i-1];
    while (pos != -1 && pat[pos] != pat[i-1]) 
      pos = T[pos];
    T[i] = pos + 1;
  }
}

int find_pattern(const string &s, const string &pat, const vector<int> &T) {
  int sp = 0, kp = 0;
  int slen = s.length(), plen = pat.length();
  while (sp < slen) {
    while (kp != -1 && (kp == plen || pat[kp] != s[sp]))  kp = T[kp];
    kp++;   sp++;
    if (kp == plen) 
      return sp - plen;   // continue with kp = T[kp] for more
  }
  return -1;  // not found
}
