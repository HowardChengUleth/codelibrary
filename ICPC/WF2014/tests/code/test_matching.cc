#include "everything"
#include "../../matching.cc"

const int MAX_N = 1030;
int A[MAX_N][MAX_N];
Node U[MAX_N],V[MAX_N];

int main(){
  int m,n;
  while(cin >> m >> n){
    for(int i=0;i<m;i++) U[i].adj.clear();
    for(int i=0;i<n;i++) V[i].adj.clear();
    
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	cin >> A[i][j];
    
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	if(A[i][j])
	  add_edge(U,i,V,j);

    int x = match(U,m,V,n);
    cout << x << endl;

    vector<int> cU,cV;
    int y = vertex_cover(U,m,V,n,cU,cV);
    if(x != y){
      cout << "WRONG! Konig's Theorem fail!" << endl;
      cout << cU.size() << "+" << cV.size() << " != " << x << endl;
      cout << m-cU.size() << " " << n-cV.size() << endl;
    }

/*    for(int i=0;i<cU.size();i++)
      cout << cU[i] << " "; cout << endl;
    for(int i=0;i<cV.size();i++)
      cout << cV[i] << " "; cout << endl;
*/    
    for(int i=0;i<cU.size();i++)
      for(int j=0;j<n;j++)
	A[cU[i]][j] = 0;
    
    for(int i=0;i<cV.size();i++)
      for(int j=0;j<m;j++)
	A[j][cV[i]] = 0;
    
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	if(A[i][j])
	  cout << "WRONG! Vertices do not cover everything!" << endl;
  }
  return 0;
}
