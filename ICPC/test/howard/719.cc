#include <iostream>
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

#define MAXLEN 10500


int find_worst(int *sarray, int *lcp, int len)
{
  int i, n, d;

  n = 2*len;
  
  /* first look for the lexicographically smallest one within the first
     part of the string */
  for (i = 0; i < n && sarray[i] >= len; i++) ;

  /* now we have to look for tie-breakers */
  d = sarray[i];
  for ( ; i < n-1 && lcp[i+1] >= len; i++) {
    if (sarray[i+1] < d) {
      d = sarray[i+1];
    }
  }
  return d+1;   /* strings are 1-based */
}

int main(void)
{
  string str, str2;
  int sarray[2*MAXLEN], lcp[2*MAXLEN];
  int n;

  cin >> n;
  while (n-- > 0) {
    cin >> str;
    str2 = str + str;
    build_sarray(str2, sarray);
    compute_lcp(str2, sarray, lcp);
    cout << find_worst(sarray, lcp, str.length()) << endl;
  }
  return 0;
}
