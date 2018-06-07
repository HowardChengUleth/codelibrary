#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <tr1/tuple>
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

// O(n log n) -- Will work for n <= 10^6
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


#include <set>
#include <vector>

int sarray[100001], lcp[100001];
vector<pair<int,int> > info;
vector<int> pushed[50001];

void do_case(int k)
{
  string str, line;
  getline(cin, line);
  while (getline(cin, line) && line != "END TDP CODEBASE") {
    str += line;
    str += '\n';
  }
  int len1 = str.length();
  getline(cin, line);
  bool first = true;
  while (getline(cin, line) && line != "END JCN CODEBASE") {
    if (first) {
      str += (char)(1);
    }
    first = false;
    str += line;
    str += '\n';
  }

  build_sarray(str, sarray);
  compute_lcp(str, sarray, lcp);
  info.clear();

  for (unsigned int i = 1; i < str.length(); i++) {
    int pos2 = sarray[i];
    if (pos2 < len1) continue;

    int best_l = -1;
    int l = lcp[i];
    for (int j = i; j > 0 && l > 0; j--) {
      int pos1 = sarray[j-1];
      if (pos1 < len1) {
	best_l = max(best_l, l);
      }
      l = min(l, lcp[j-1]);
    }

    if (i < str.length()-1) {
      l = INT_MAX;
      for (unsigned int j = i; j < str.length()-1; j++) {
	l = min(l, lcp[j+1]);
	if (l == 0) break;
	int pos1 = sarray[j+1];
	if (pos1 < len1) {
	  best_l = max(best_l, l);
	}
      }
    }
    if (best_l >= 0) {
      info.push_back(make_pair(-best_l, pos2));
    }
  }

  sort(info.begin(), info.end());

  pair<int, int> segments[100001];

  int segment = 0;
  for (vector<pair<int, int> >::iterator it = info.begin();
       it != info.end() && segment < k;
       ++it) {
    int len = -it->first;
    int pos = it->second;

    // already contained in another segment
    bool okay = true;
    for (int j = 0; j < segment; j++) {
      if (segments[j].first <= pos &&
	  pos + len <= segments[j].first + segments[j].second) {
	okay = false;
      }
    }
    if (!okay) continue;

    segments[segment] = make_pair(pos, len);
    cout << "INFRINGING SEGMENT " << ++segment << " LENGTH "
	 << len << " POSITION " << pos - len1 - 1 << endl;
    for (int j = 0; j < len; j++) {
      cout << str[pos+j];
    }
    cout << endl;
    
  }
}

int main(void)
{
  int case_num = 1;
  int k;
  while (cin >> k && k) {
    cin.ignore(1);
    if (case_num > 1) {
      cout << endl;
    }
    cout << "CASE " << case_num++ << endl;
    do_case(k);
  }
   
  return 0;
}
