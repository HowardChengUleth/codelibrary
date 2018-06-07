#include "everything"
#include "../../hungarian.cc"

int G[MAX_N][MAX_N];

int main(void){
  int matching[MAX_N];
  int n;

  while(cin >> n){
    for(int i=0;i<n;i++)
      for(int j=0;j<n;j++)
	cin >> G[i][j];
    cout << "Min: " << min_weight_matching(G,n,matching) << endl;
    for(int i=0;i<n;i++) cout << matching[i] << " ";
    cout << endl;
    cout << "Max: " << max_weight_matching(G,n,matching) << endl;
    for(int i=0;i<n;i++) cout << matching[i] << " ";
    cout << endl;
  }
  return 0;
}
