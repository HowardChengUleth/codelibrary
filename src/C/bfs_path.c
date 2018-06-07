/*
 * Shortest Path with BFS
 *
 * Author: Howard Cheng
 *
 * Given a graph represented by an adjacency list, this algorithm uses
 * BFS to find the shortest path from a source vertex to each of the
 * other vertices.  The distances from the source to v is given in D[v], and
 * D[v] is set to -1 if the source vertex is not connected to w.  Also,
 * the shortest path tree is stored in the array P.
 *
 * Call get_path to recover the path.
 * 
 * Note: All edges must have the same cost for this to work.
 *       This algorithm has complexity O(n+m).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NODES 100

typedef struct {
  int deg;           /* number of outgoing edges */
  int adj[MAX_NODES];
  
  /* the following is not necessary, but useful in many situations */
  int cost[MAX_NODES];
} Node;

int max(int x, int y)
{
  return (x > y) ? x : y;
}

void BFS_shortest_path(Node graph[MAX_NODES], int n, int src, int D[MAX_NODES],
		       int P[MAX_NODES])
{
  char used[MAX_NODES];
  int queue[MAX_NODES+1];
  int q_in, q_out, i;
  int v, w;

  memset(used, 0, MAX_NODES);
  queue[0] = src;
  used[src] = 1;
  q_in = 1;
  q_out = 0;
  
  for (i = 0; i < MAX_NODES; i++) {
    D[i] = -1;
    P[i] = -1;
  }
  D[src] = 0;

  while (q_out < q_in) {
    v = queue[q_out++];
    for (i = 0; i < graph[v].deg; i++) {
      w = graph[v].adj[i];
      if (!used[w]) {
	D[w] = D[v] + 1;
	P[w] = v;
	queue[q_in++] = w;
	used[w] = 1;
      } else if (D[v] + 1 == D[w]) {
	/* put tie-breaker here */
	/* eg. find largest path in lexicographic order, when the path */
        /*     is considered in REVERSE!                               */
	P[w] = max(P[w], v);
      }
    }
  }
}

void clear(Node graph[MAX_NODES], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    graph[i].deg = 0;
  }
}

void add_edge(Node graph[MAX_NODES], int v, int w, int cost)
{
  int i;

  /* make sure that we have no duplicate edges */
  for (i = 0; i < graph[v].deg; i++) {
    if (graph[v].adj[i] == w) {
      assert(0);
    }
  }

  graph[v].cost[graph[v].deg] = cost;
  graph[v].adj[graph[v].deg] = w;
  graph[v].deg++;
}

int get_path(int v, int *P, int *path)
{
  int A[MAX_NODES];
  int i, k;

  k = 0;
  A[k++] = v;
  while (P[v] != -1) {
    v = P[v];
    A[k++] = v;
  }
  for (i = k-1; i >= 0; i--) {
    path[k-1-i] = A[i];
  }
  return k;
}

int main(void)
{
  int v, w, num;
  int i;
  Node graph[MAX_NODES];
  int P[MAX_NODES][MAX_NODES], D[MAX_NODES][MAX_NODES];
  int path[MAX_NODES];

  setbuf(stdout, 0);
  clear(graph, MAX_NODES);
  while (scanf("%d %d\n", &v, &w) == 2 && v >= 0 && w >= 0) {
    add_edge(graph, v, w, 1);
  }
  
  for (i = 0; i < MAX_NODES; i++) {
    BFS_shortest_path(graph, MAX_NODES, i, D[i], P[i]);
  }

  while (scanf("%d %d", &v, &w) == 2 && v >= 0 && w >= 0) {
    printf("%d %d: %d\n", v, w, D[v][w]);
    num = get_path(w, P[v], path);
    assert(D[v][w] == -1 || num == D[v][w]+1);
    for (i = 0; i < num; i++) {
      printf(" %d", path[i]);
    }
    printf("\n");
  }
  return 0;

}
