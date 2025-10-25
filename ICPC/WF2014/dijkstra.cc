// Dijkstra's Algorithm [Dense Graphs] (Directed and Undirected) -- O(n^2)
//  -- Edge weight >= 0. May use get_path to obtain the path.

void dijkstra(int graph[MAX_N][MAX_N], int n, int src, int D[], int P[]) {
  char used[MAX_N];
  int fringe[MAX_N], f_size, v, w, j, wj, best, best_init;

  f_size = 0;
  for (v = 0; v < n; v++) 
    if (graph[src][v] != DISCONNECT && src != v) {
      D[v] = graph[src][v];    P[v] = src;   fringe[f_size++] = v;    used[v] = 1;
    } else {
      D[v] = DISCONNECT;  P[v] = -1;  used[v] = 0;
    }
  
  D[src] = 0;   P[src] = -1;  used[src] = 1;  best_init = 1;
  while (best_init) {
    best_init = 0;
    for (j = 0; j < f_size; j++) {
      v = fringe[j];
      if (!best_init || D[v] < best) {
        best = D[v];  w = v;  wj = j;  best_init = 1;
      }
    }

    if (best_init) {
      f_size--;

      for (j = wj; j < f_size; j++)   fringe[j] = fringe[j+1];
      for (v = 0; v < n; v++) 
        if (v != src && graph[w][v] != DISCONNECT) {
          if (D[v] == DISCONNECT || D[w] + graph[w][v] < D[v]) {
            D[v] = D[w] + graph[w][v];    P[v] = w;
          } else if (D[w] + graph[w][v] == D[v]) {
          }  /* put tie-breaker here */
          if (!used[v]) {
            used[v] = 1;        fringe[f_size++] = v;
          }
        }
    }
  }
  D[src] = 0;
}
