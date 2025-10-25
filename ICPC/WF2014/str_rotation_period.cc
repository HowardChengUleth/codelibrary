// Find lex least rotation of a string, and smallest period of a string: O(n)
// pos = start of lex least rotation, period = the period
void compute(string s, int &pos, int &period) {
  s += s;
  int len = s.length(), i = 0, j = 1;
  for (int k = 0; i+k < len && j+k < len; k++) {
    if (s[i+k] > s[j+k]) {
      i = max(i+k+1, j+1);      k = -1;
    } else if (s[i+k] < s[j+k]) {
      j = max(j+k+1, i+1);      k = -1;
    }
  }
  pos = min(i, j);
  period = (i > j) ? i - j : j - i;
}
