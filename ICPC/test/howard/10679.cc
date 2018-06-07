
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <climits>
#include <vector>
#include <tr1/tuple>

using namespace std;
using namespace std::tr1;

typedef pair<int,int> pii;
typedef tuple<int,int,int> tiii;
typedef vector<int> vi;

void radixPass(vi &a, vi &b, vi &r, int n, int K, int off=0) {
  vi c(K+1, 0);
  for (int i = 0;  i < n;  i++) c[r[a[i]+off]]++;
  for (int i = 0, sum = 0;  i <= K;  i++) {
     int t = c[i];  c[i] = sum;  sum += t;
  }
  for (int i = 0;  i < n;  i++) b[c[r[a[i]+off]]++] = a[i];
}

#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
void sarray_int(vi &s, vi &SA, int n, int K) {
  int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2; 
  vi s12(n02 + 3, 0), SA12(n02 + 3, 0), s0(n0), SA0(n0);
 
  for (int i=0, j=0;  i < n+(n0-n1);  i++) if (i%3 != 0) s12[j++] = i;

  radixPass(s12 , SA12, s, n02, K, 2);
  radixPass(SA12, s12 , s, n02, K, 1);  
  radixPass(s12 , SA12, s, n02, K, 0);

  int name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (int i = 0;  i < n02;  i++) {
    if (s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2) { 
      name++;  c0 = s[SA12[i]];  c1 = s[SA12[i]+1];  c2 = s[SA12[i]+2];
    }
    if (SA12[i] % 3 == 1) { s12[SA12[i]/3]      = name; }
    else                  { s12[SA12[i]/3 + n0] = name; }
  }

  if (name < n02) {
    sarray_int(s12, SA12, n02, name);
    for (int i = 0;  i < n02;  i++) s12[SA12[i]] = i + 1;
  } else
    for (int i = 0;  i < n02;  i++) SA12[s12[i] - 1] = i; 

  for (int i=0, j=0;  i < n02;  i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
  radixPass(s0, SA0, s, n0, K);

  for (int p=0,  t=n0-n1,  k=0;  k < n;  k++) {
    int i = GetI(), j = SA0[p];
    if (SA12[t] < n0 ? 
        (pii(s[i], s12[SA12[t] + n0]) < pii(s[j], s12[j/3])) :
	(tiii(s[i],s[i+1],s12[SA12[t]-n0+1]) < tiii(s[j],s[j+1],s12[j/3+n0]))) {
      SA[k] = i;  t++;
      if (t == n02) 
        for (k++;  p < n0;  p++, k++) SA[k] = SA0[p];
    } else { 
      SA[k] = j;  p++; 
      if (p == n0) 
        for (k++;  t < n02;  t++, k++) SA[k] = GetI(); 
    }  
  } 
}

void build_sarray(string str, int sarray[]) {
  int n = str.length();
  if (n <= 1) {
    for (int i = 0; i < n; i++)  sarray[i] = i;
    return;
  }

  vi s(n+3, 0), SA(n+3);
  for (int i = 0; i < n; i++)
    s[i] = (int)str[i] - CHAR_MIN + 1;
  sarray_int(s, SA, n, 256);
  copy(SA.begin(), SA.begin()+n, sarray);
}

void compute_lcp(string str, int sarray[], int lcp[]) {
  int n = str.length();
  vi rank(n);
  for (int i = 0; i < n; i++)   rank[sarray[i]] = i;

  int h = 0;
  for (int i = 0; i < n; i++) {
    int k = rank[i];
    if (k == 0) {
      lcp[k] = -1;
    } else {
      int j = sarray[k-1];
      while (i + h < n && j + h < n && str[i+h] == str[j+h]) h++;
      lcp[k] = h;
    }
    if (h > 0) h--;
  }
  lcp[0] = 0;
}

#include <cstring>

struct Comp
{
  const char *s;
  Comp(const string &str) : s(str.c_str()) { }
  bool operator()(int i, const string &p) const {
    return strncmp(s, p.c_str(), p.length()) < 0;
  }
  bool operator()(const string &p, int i) const {
    return strncmp(p.c_str(), s, p.length()) < 0;
  }
};

pii find(const string &str, const int sarray[], const string &pattern) {
  int n = str.length();
  pair<const int *, const int *> p = 
    equal_range(sarray, sarray+n, pattern, Comp(str));
  return pii(p.first - sarray, p.second - sarray);
}
int main(void)
{
  string S, T;
  int sarray[100001];
  int k, q, n;

  cin >> k;
  while (k-- > 0) {
    cin >> S;
    build_sarray(S, sarray);
    cin >> q;
    while (q-- > 0) {
      cin >> T;

      pair<int,int> res = find(S, sarray, T);
      if (res.first < res.second) {
	cout << "y" << endl;
      } else {
	cout << "n" << endl;
      }
      /*
      for (int i = res.first; i != res.second; i++) {
	cout << S.substr(sarray[i], string::npos) << endl;
	}
      */
    }
  }

  return 0;
}
