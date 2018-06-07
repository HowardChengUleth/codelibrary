/*
 * KMP String Matching
 *
 * Author: Howard Cheng
 *
 * The prepare_pattern routine takes in the pattern you wish to search
 * for, and perform some processing to give a "failure array" to be used
 * by the actual search.  The complexity is linear in the length of the
 * pattern.
 *
 * The find_pattern routine takes in a string s, a pattern pat, and a
 * vector T computed by prepare_pattern.  It returns the index of the
 * first occurrence of pat in s, or -1 if it does not occur in s.
 * The complexity is linear in the length of the string s.
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void prepare_pattern(const string &pat, vector<int> &T)
{
  int n = pat.length();
  T.resize(n+1);
  fill(T.begin(), T.end(), -1);
  for (int i = 1; i <= n; i++) {
    int pos = T[i-1];
    while (pos != -1 && pat[pos] != pat[i-1]) {
      pos = T[pos];
    }
    T[i] = pos + 1;
  }
}

int find_pattern(const string &s, const string &pat, const vector<int> &T)
{
  int sp = 0, kp = 0;
  int slen = s.length(), plen = pat.length();
  while (sp < slen) {
    while (kp != -1 && (kp == plen || pat[kp] != s[sp])) {
      kp = T[kp];
    }
    kp++;   sp++;
    if (kp == plen) {
      // a match is found
      return sp - plen;

      // if you want more than one match (i.e. all matches), do not return
      // in the above but rather record the location of the match.  Continue
      // the loop with:
      //
      // kp = T[kp];
    }
  }
  return -1;
}

int main(void)
{
  string str, pat;

  while (cin >> str >> pat) {
    vector<int> T;
    prepare_pattern(pat, T);
    cout << "index = " << find_pattern(str, pat, T) << endl;
  }
  return 0;
}
