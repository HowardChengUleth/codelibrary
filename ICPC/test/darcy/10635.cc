#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_N = 251*251;
int A[MAX_N],B[MAX_N];

int asc_seq(int A[], int n, int S[]) {
  vector<int> last(n+1), pos(n+1), pred(n);

  if (n == 0) return 0;
  int len = 1;                  last[1] = A[pos[1] = 0];
  for (int i = 1; i < n; i++) {
    int j = lower_bound(last.begin()+1, last.begin()+len+1, A[i]) -
      last.begin();      // use lower_bound for strict increasing subsequence
    pred[i] = (j-1 > 0) ? pos[j-1] : -1;
    last[j] = A[pos[j] = i];    len = max(len, j);
  }
  int start = pos[len];
  for (int i = len-1; i >= 0; i--) {  S[i] = A[start];  start = pred[start];  }
  return len;
}

int main(){
  int C=1,T,x,n,q,p;
  cin >> T;
  while(T--){
    cin >> n >> p >> q;
    for(int i=0;i<=n*n;i++)
      A[i] = -1;
    for(int i=0;i<=p;i++){
      cin >> x;
      A[x] = i;
    }
    int idx = 0;
    for(int j=0;j<=q;j++){
      cin >> x;
      if(A[x] != -1) B[idx++] = A[x];
    }
    cout << "Case " << C++ << ": " << asc_seq(B,idx,A) << endl;
  }
  return 0;
}
