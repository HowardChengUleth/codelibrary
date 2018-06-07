/*
 * Floyd's Algorithm
 *
 * Author: Howard Cheng
 *
 * The following code takes a graph stored in an adjacency matrix "graph",
 * and returns the shortest distance from node i to node j in dist[i][j].
 * We assume that the weights of the edges is not DISCONNECT, and the
 * DISCONNECT constant is used to indicate the absence of an edge.
 *
 * Call extract_path to return the path, as well as its length (in terms
 * of vertices).  The length is -1 if no such path exists.
 *
 */

#include <iostream>
#include <cassert>

using namespace std;

const int MAX_NODES = 26;
const int DISCONNECT = -1;

int graph[MAX_NODES][MAX_NODES];
int dist[MAX_NODES][MAX_NODES];
int succ[MAX_NODES][MAX_NODES];

void floyd(void)
{
  int i, j, k;

  for (i = 0; i < MAX_NODES; i++) {
    for (j = 0; j < MAX_NODES; j++) {
      dist[i][j] = graph[i][j];
      if (i == j || graph[i][j] == DISCONNECT) {
	succ[i][j] = -1;
      } else {
	succ[i][j] = j;
      }
    }
  }

  for (k = 0; k < MAX_NODES; k++) {
    for (i = 0; i < MAX_NODES; i++) {
      for (j = 0; j < MAX_NODES; j++) {
        if (i != k && dist[i][k] != DISCONNECT && dist[k][j] != DISCONNECT) {
          int temp = dist[i][k] + dist[k][j];
          if (dist[i][j] == DISCONNECT || dist[i][j] > temp) {
            dist[i][j] = temp;
	    succ[i][j] = succ[i][k];
          } else if (dist[i][j] == temp && succ[i][k] < succ[i][j]) {
            /* put tie-breaking on paths here */

	    /* e.g. the test above chooses lexicographically smallest */
	    /*      paths, but ignores the number of vertices in the  */
	    /*      path.  To really do lexicographically sorting     */
	    /*      properly, you also need to have len[i][j] which   */
	    /*      can be computed easily as well.                   */
	    succ[i][j] = succ[i][k];
	  }
        }
      }
    }
  }

  for (i = 0; i < MAX_NODES; i++) {
    dist[i][i] = 0;
  }
}

int extract_path(int u, int v, int path[])
{
  int len = 0;
  
  if (dist[u][v] == DISCONNECT) {
    return -1;
  }

  path[len++] = u;
  while (u != v) {
    u = succ[u][v];
    path[len++] = u;
  }

  return len;
}

int main(void)
{
  int m, w, i, j;
  int path[MAX_NODES], len;

  /* clear graph */
  for (i = 0; i < MAX_NODES; i++) {
    for (j = 0; j < MAX_NODES; j++) {
      graph[i][j] = DISCONNECT;
    }
  }

  /* read graph */
  cin >> i >> j >> w;
  while (!(i == -1 && j == -1)) {
    assert(0 <= i && i < MAX_NODES && 0 <= j && j < MAX_NODES);
    graph[i][j] = /*graph[j][i] =*/ w;
    cin >> i >> j >> w;
  }

  floyd();

  /* do queries */
  cin >> i >> j;
  while (!(i == -1 && j == -1)) {
    assert(0 <= i && i < MAX_NODES && 0 <= j && j < MAX_NODES);
    cout << i << " " << j << ": " << dist[i][j] << endl;
    len = extract_path(i, j, path);
    for (m = 0; m < len; m++) {
      if (m) {
	cout << " ";
      }
      cout << path[m];
    }
    cout << endl;
    cin >> i >> j;
  }

  return 0;
}
