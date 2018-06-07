

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>
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

int main(void)
{
  string str;
  while (getline(cin, str) && str != "") {
    for (int i = str.length()-1; i >= 0; i--) {
      if (str[i] == ' ') {
	str.erase(i, 1);
      }
    }
    
    int sarray[1000], lcp[1000];
    build_sarray(str, sarray);
    compute_lcp(str, sarray, lcp);

    int max_len = 0;
    int count[1001] = {0};
    int start[1001];
    fill(start, start+1001, -1);

    for (unsigned int i = 1; i < str.length(); i++) {
      max_len = max(lcp[i], max_len);
      if (lcp[i-1] == lcp[i]) {
	// nothing to do
      } else if (lcp[i-1] < lcp[i]) {
	fill(start+lcp[i-1]+1, start+lcp[i]+1, i-1);
      } else {
	for (int j = lcp[i]+1; j <= lcp[i-1]; j++) {
	  count[j] = max(count[j], (int)i - start[j]);
	  start[j] = -1;
	}
      }
    }

    // deal with the last one
    if (lcp[str.length()-1] > 0) {
      for (int j = 1; j <= lcp[str.length()-1]; j++) {
	count[j] = max(count[j], (int)str.length() - start[j]);
      }
    }

    for (int i = 1; i <= max_len; i++) {
      cout << count[i] << endl;
    }
    cout << endl;
  }
  return 0;
}
