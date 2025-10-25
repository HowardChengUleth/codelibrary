// Floyd's Algorithm with path information (Undirected and Directed) -- O(n^3)
//  -- Length = -1 if no path exists
const int DISCONNECT = -1;

int graph[MAX_N][MAX_N], dist[MAX_N][MAX_N], succ[MAX_N][MAX_N];

void floyd(int n){
  for (int i = 0; i < n; i++) 
    for (int j = 0; j < n; j++) {
      dist[i][j] = graph[i][j];
      succ[i][j] = (i == j || graph[i][j] == DISCONNECT) ? -1 : j;
    }

  for (int k = 0; k < n; k++) 
    for (int i = 0; i < n; i++) 
      for (int j = 0; j < n; j++) 
        if (i != k && dist[i][k] != DISCONNECT && dist[k][j] != DISCONNECT) {
          int temp = dist[i][k] + dist[k][j];
          if (dist[i][j] == DISCONNECT || dist[i][j] > temp) {
            dist[i][j] = temp;     succ[i][j] = succ[i][k];
          } else if (dist[i][j] == temp && succ[i][k] < succ[i][j]) {
            // put tie-breaking on paths here: the one above kind of
	    // chooses lex smallest path (but ignores the number of
	    // vertices in the path!)
	    succ[i][j] = succ[i][k];
	  }
        }

  for (int i = 0; i < n; i++) dist[i][i] = 0;
}

int extract_path(int u, int v, int path[]) {
  int len = 0;
  if (dist[u][v] == DISCONNECT) return -1;

  path[len++] = u;
  while (u != v) {
    u = succ[u][v];    path[len++] = u;
  }
  return len;
}
