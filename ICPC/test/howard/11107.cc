

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cassert>
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


const int STR_LEN = 1024;

int string_index(int k)
{
  return k / STR_LEN;
}

void do_case(int n)
{
  string str;
  for (int i = 0; i < n; i++) {
    string part;
    cin >> part;
    str += part;
    str += string(STR_LEN - part.length(), (char)(128 + i));
  }

  const int MAX_LEN = STR_LEN*100;
  int sarray[MAX_LEN], lcp[MAX_LEN];

  build_sarray(str, sarray);
  compute_lcp(str, sarray, lcp);

  int longest = 0;
  int count = 0;
  int occurs[100];
  fill(occurs, occurs+n, 0);

  int i = 0, s = 0;
  while (i < n * STR_LEN) {
    // let's collect until we have more than half, note that i is one too
    // big when we exit the loop
    for (; count <= n/2 && i < n * STR_LEN; i++) {
      if (!islower(str[sarray[i]])) continue;
      if (!occurs[string_index(sarray[i])]) {
	count++;
      }
      occurs[string_index(sarray[i])]++;
    }

    // now try to move the start of the range so that it is just <= half
    for (; count > n/2; s++) {
      if (!islower(str[sarray[s]])) continue;
      if (--occurs[string_index(sarray[s])] == 0) {
	count--;
      }
    }
    
    // now we want to know how many are in the range from s-1 to i-1
    int curr_len = *min_element(lcp+s, lcp+i);
    longest = max(longest, curr_len);
  }

  if (longest == 0) {
    cout << "?" << endl;
    return;
  }

  // now we know the longest length, so we go and recover them
  i = s = 0;
  count = 0;
  string prev;
  fill(occurs, occurs+n, 0);
  while (i < n * STR_LEN) {
    for (; count <= n/2 && i < n * STR_LEN; i++) {
      if (!islower(str[sarray[i]])) continue;
      if (!occurs[string_index(sarray[i])]) {
	count++;
      }
      occurs[string_index(sarray[i])]++;
    }

    // now try to move the start of the range so that it is just <= half
    for (; count > n/2; s++) {
      if (!islower(str[sarray[s]])) continue;
      if (--occurs[string_index(sarray[s])] == 0) {
	count--;
      }
    }
    
    // now we want to know how many are in the range from s-1 to i-1
    int curr_len = *min_element(lcp+s, lcp+i);
    if (curr_len == longest) {
      // because suffix arrays are sorted in alphabetical order, the
      // only way we can print something twice is if it is exactly the same
      // as the previous one, up to the "longest" characters
      if (prev != str.substr(sarray[s-1], longest)) {
	prev = str.substr(sarray[s-1], longest);
	cout << prev << endl;
      }
    }
  }
}

int main(void)
{
  int n;
  bool first = true;
  while (cin >> n && n > 0) {
    if (!first) {
      cout << endl;
    }
    first = false;
    do_case(n);
  }
  return 0;
}
