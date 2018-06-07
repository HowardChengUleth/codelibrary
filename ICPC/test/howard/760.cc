#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>
#include <tr1/tuple>
#include <vector>
#include <climits>

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
  if (n <= 1) { for (int i = 0; i < n; i++)  sarray[i] = i; return; }

  vi RA(2*n, 0), SA(2*n), tmp(2*n);
  for (int i = 0; i < n; i++) RA[i] = (int)str[i] - CHAR_MIN + 1;
  for (int i = 0; i < n; i++) SA[i] = i;
  for (int k = 1; k < n; k <<= 1){
    radixPass(SA,tmp,RA,n,max(n,256),k);
    radixPass(tmp,SA,RA,n,max(n,256),0);
    tmp[SA[0]] = 1;
    for(int i=1;i<n;i++){
      tmp[SA[i]] = tmp[SA[i-1]];
      if((RA[SA[i]] != RA[SA[i-1]]) || (RA[SA[i]+k] != RA[SA[i-1]+k]))
        tmp[SA[i]]++;
    }
    copy(tmp.begin(),tmp.begin()+n,RA.begin());
  }
  copy(SA.begin(), SA.begin()+n, sarray);
}

// O(n)
void compute_lcp(string str, int sarray[], int lcp[]) {
  int n = str.length(), h = 0;  vi rank(n);
  for (int i = 0; i < n; i++)   rank[sarray[i]] = i;
  
  for (int i = 0; i < n; i++) {
    int k = rank[i];
    if (k > 0) {
      int j = sarray[k-1];
      while (i + h < n && j + h < n && str[i+h] == str[j+h]) h++;
      lcp[k] = h;
    }
    if (h > 0) h--;
  }
  lcp[0] = 0;
}

struct Comp{
  const string &s; int m;
  Comp(const string &str,int M) : s(str),m(M) { }
  bool operator()(int i, const string& p) const { return s.compare(i,m,p,0,m) < 0; }
  bool operator()(const string& p, int i) const { return s.compare(i,m,p,0,m) > 0; }
};

pii find(const string &str, const int sarray[], const string &pattern) {
  pair<const int *, const int *> p = 
    equal_range(sarray, sarray+str.size(), pattern, Comp(str,pattern.size()));
  return pii(p.first - sarray, p.second - sarray);
}



#include <iostream>
#include <set>
#include <string>

using namespace std;

void solve(const string &s1, const string &s2)
{
  int n1 = s1.length(), n2 = s2.length();
  string str = s1 + "#" + s2;
  int sarray[601], lcp[601];
  build_sarray(str, sarray);
  compute_lcp(str, sarray, lcp);

  int best = 0;
  for (int i = 1; i < n1+n2+1; i++) {
    if (((sarray[i-1] < n1 && sarray[i] >= n1) ||
	 (sarray[i-1] >= n1 && sarray[i] < n1))) {
      best = max(best, lcp[i]);
    }
  }

  if (best == 0) {
    cout << "No common sequence." << endl;
  } else {
    set<string> S;
    for (int i = 1; i < n1+n2+1; i++) {
      if (((sarray[i-1] < n1 && sarray[i] >= n1) ||
	   (sarray[i-1] >= n1 && sarray[i] < n1)) &&
	  best == lcp[i]) {
	S.insert(str.substr(sarray[i], best));
      }
    }

    for (set<string>::const_iterator it = S.begin(); it != S.end(); ++it) {
      cout << *it << endl;
    }
  }
}

int main(void)
{
  string s1, s2;
  bool first = true;
  while (cin >> s1 >> s2) {
    if (!first) cout << endl;
    solve(s1, s2);
    first = false;
  }
  
  return 0;
}
