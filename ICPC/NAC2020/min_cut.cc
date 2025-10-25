// Minimum Cut (Undirected Only) -- O(n^3)
int min_cut(int G[MAX_N][MAX_N],int n){ // DISCONNECT == 0
  int w[MAX_N],p,j,J,best = -1,A[MAX_N];
  
  for(n++ ; n-- ; ){
    fill(A,A+n,true), A[p = 0] = false, copy(G[0],G[0]+n,w);
    for(int i=1;i<n;i++){
      for(j=1,J=0;j<n;j++) if(A[j] && (!J || w[j] > w[J])) J = j;
      A[J] = false;
      if(i == n-1){
	if(best < 0 || best > w[J]) best = w[J];
	for(int i=0;i<n;i++) G[i][p] = G[p][i] += G[i][J];
	for(int i=0;i<n-1;i++) G[i][J] = G[J][i]  = G[i][n-1];
	G[J][J] = 0;
      }
      for(p=J,j=1;j<n;j++) if(A[j]) w[j] += G[J][j];
    }
  }
  return best;
}
