void match(int A[MAX_N][MAX_N],int P[],int q,int& ans){
  if(q == 2) {
    int tmp = 0;
    for(int i=0;i<q;i+=2) tmp += A[P[i]][P[i+1]];
    if(ans == -1 || ans > tmp) ans = tmp;
    return;
  }
  match(A,P,q-2,ans);
  for(int i=q-3;i>=0;i--)
    swap(P[q-2],P[i]), match(A,P,q-2,ans);
  rotate(P,P+q-2,P+q-1);
}

// Input is a vector of edges (u,v,c) <-- Edge of cost c between u and v
int postman(vector<tuple<int,int,int> > V,int n){
  int u,v,c,sum = 0, deg[MAX_N] = {0}, P[MAX_N], q=0, A[MAX_N][MAX_N];
  for(int i=0;i<n;i++) fill(A[i],A[i]+n,-1);
  for(int i=0;i<V.size();i++){
    tie(u,v,c) = V[i];  deg[u]++; deg[v]++; sum += c;
    if(A[u][v] == -1 || A[u][v] > c) A[u][v] = A[v][u] = c;
  }
  for(int k=0;k<n;k++)
    for(int i=0;i<n;i++)
      for(int j=0;j<n;j++){
	if(A[i][k] == -1 || A[k][j] == -1) continue;
	if((A[i][j] == -1) || (A[i][j] > A[i][k] + A[k][j]))
	  A[i][j] = A[i][k]+A[k][j];
      }

  for(int i=0;i<n;i++) if(deg[i]%2) P[q++] = i;
  if(!q) return sum; // Eulerian Tour!
  int ans = -1;
  match(A,P,q,ans);
  return sum+ans;
}
