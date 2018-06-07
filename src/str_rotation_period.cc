/*
 * Finding the lexicographically least rotation of a string, and finding
 * the smallest period of a string.
 *
 * Author: Sumudu Fernando
 *
 * Given a string, the algorithm can be used to compute two things:
 *
 *   a) the position at which the lexicographically least rotation starts.
 *      If there are ties, give the first position.
 *   b) the length of the shortest substring such that the original string
 *      is a concatenation of copies of that substring
 *
 * Complexity: O(n) where n = length of the string
 *
 * Tested on: 719            Glass Beads
 *            10298          Power Strings
 *            ACPC 2011 H    Let's call a SPaDE a SPaDE
 */

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// pos = position of the start of the lexicographically least rotation
// period = the period
void compute(string s, int &pos, int &period)
{
  s += s;
  int len = s.length();
  int i = 0, j = 1;
  for (int k = 0; i+k < len && j+k < len; k++) {
    if (s[i+k] > s[j+k]) {
      i = max(i+k+1, j+1);
      k = -1;
    } else if (s[i+k] < s[j+k]) {
      j = max(j+k+1, i+1);
      k = -1;
    }
  }

  pos = min(i, j);
  period = (i > j) ? i - j : j - i;
}

int main(void)
{
  string s;
  while (cin >> s) {
    int pos, period;
    compute(s, pos, period);
    int n = s.length();
    s += s;
    cout << "least rotation = " << s.substr(pos, n) << endl;
    cout << "period = " << s.substr(0, period) << endl;
  }
  return 0;
}

