// Hungarian Algorithm (Undirected Only) -- O(n^3)
// Each half of the graph has exactly N vertices (0 to N-1).
// G[i][j] = weight (left_i, right_j), matching[i] = right vertex matched to left_i
//  -- If the set of U and V are different sizes, pad the smaller one with enough
//     nodes to make them equal sizes. Connect these nodes that you have added with
//     EVERY node in the larger one with a weight of 0.
//   Absent edge: DISCONNECT
const int DISCONNECT = INT_MIN;

// Global Variables (Used internally):
int lx[MAX_N],ly[MAX_N],slack[MAX_N],slack_x[MAX_N],pre[MAX_N],revmatch[MAX_N];
bool S[MAX_N], T[MAX_N];

bool locate_path(int& x,int& y,int G[MAX_N][MAX_N],int N,queue<int>& q,bool phase1){
  if(phase1){ x = q.front(); q.pop(); }
  for (y = 0; y < N; y++){
    if(!phase1) x = slack_x[y];
    if (T[y]) continue;
    if((phase1 && G[x][y] == lx[x] + ly[y]) || (!phase1 && slack[y] == 0)){
      if (revmatch[y] == -1) return true;
      T[y] = true;
      if(phase1 || !S[revmatch[y]]){
	q.push(revmatch[y]);
	int tmp;   S[revmatch[y]] = true;  pre[revmatch[y]] = x;
	x = revmatch[y];
	for(int i=0;i<N;i++)
	  if(G[x][i] != DISCONNECT && (tmp = lx[x] + ly[i] - G[x][i]) < slack[i])
	    slack[i] = tmp , slack_x[i] = x;
      }
    }
  }
  return false;
}

int max_weight_matching(int G[MAX_N][MAX_N], int N, int matching[MAX_N]) {
  fill_n(matching, N, -1);  fill_n(revmatch, N, -1); fill_n(ly, N, 0);
  for(int i=0;i<N;i++) lx[i] = *max_element(G[i], G[i]+N);

  for(int max_match=0;max_match < N;max_match++) {
    queue<int> q;
    fill_n(S, N, false);  fill_n(T, N, false);  fill_n(pre, N, -1);
    
    int root = find(matching, matching+N, -1) - matching;
    q.push(root);       pre[root] = -2;      S[root] = true;

    fill_n(slack_x, N, root);
    for (int y = 0; y < N; y++)
      slack[y] = (G[root][y]==DISCONNECT) ? INT_MAX : lx[root]+ly[y]-G[root][y];
    
    int x, y;
    while (true) {
      while(!q.empty())	if(locate_path(x,y,G,N,q,true)) goto path_found;
      int delta = INT_MAX;
      for (y = 0; y < N; y++) if (!T[y]) delta = min(delta,slack[y]);
      for (x = 0; x < N; x++) if (S[x]) lx[x] -= delta;
      for (y = 0; y < N; y++) if (T[y]) ly[y] += delta; else slack[y] -= delta;
      if(locate_path(x,y,G,N,q,false)) goto path_found;
    }
    path_found:  // <-- That is a colon, not a semi-colon
    for (int cx = x, cy = y, ty; cx != -2; cx = pre[cx], cy = ty) {
      ty = matching[cx];   revmatch[cy] = cx;   matching[cx] = cy;
    }
  }
  
  // return the final answer
  int weight = 0;
  for (int x = 0; x < N; x++) weight += G[x][matching[x]];
  return weight;
}

int min_weight_matching(int G[MAX_N][MAX_N], int N, int matching[MAX_N]) {
  int M = INT_MIN;
  for (int i = 0; i < N; i++)
    M = max(M, *max_element(G[i],G[i]+N));
  
  int newG[MAX_N][MAX_N];
  for (int i = 0; i < N; i++) 
    for (int j = 0; j < N; j++) 
      newG[i][j] = (G[i][j] == DISCONNECT) ? DISCONNECT : M - G[i][j];

  return N*M - max_weight_matching(newG, N, matching);
}
