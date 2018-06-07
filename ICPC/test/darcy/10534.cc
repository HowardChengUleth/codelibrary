#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int asc_seq(int A[], int n, int S[],int part[]) {
  vector<int> last(n+1), pos(n+1), pred(n);

  if (n == 0) return 0;
  int len = 1;                  last[1] = A[pos[1] = 0];
  part[0] = 1;
  for (int i = 1; i < n; i++) {
    int j = lower_bound(last.begin()+1, last.begin()+len+1, A[i]) -
      last.begin();      // use lower_bound for strict increasing subsequence
    pred[i] = (j-1 > 0) ? pos[j-1] : -1;
    last[j] = A[pos[j] = i];    len = max(len, j); part[i] = len;
  }
  int start = pos[len];
  for (int i = len-1; i >= 0; i--) {  S[i] = A[start];  start = pred[start];  }
  return len;
}

const int MAX_N = 10030;
int A[MAX_N],partUp[MAX_N],partDown[MAX_N],S[MAX_N];

int main(){
  int n;
  while(cin >> n){
    for(int i=0;i<n;i++)
      cin >> A[i];
    
    asc_seq(A,n,S,partUp);
    reverse(A,A+n);
    asc_seq(A,n,S,partDown);
    reverse(partDown,partDown+n);
    
    int maxi = 0;
    for(int i=0;i<n;i++)
      if(partUp[i] == partDown[i])
	if(maxi < partUp[i])
	  maxi = partUp[i];
    cout << 2*maxi-1 << endl;
  }
  return 0;
}
