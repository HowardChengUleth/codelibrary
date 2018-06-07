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

#include <iostream>
#include <cassert>
#include <algorithm>
#include <queue>

using namespace std;

const int MAX_NODES = 100;

struct Node {
  int deg;           /* number of outgoing edges */
  int adj[MAX_NODES];
  
  /* the following is not necessary, but useful in many situations */
  int cost[MAX_NODES];
};

void BFS_shortest_path(Node graph[], int n, int src, int D[], int P[])
{
  char used[MAX_NODES];
  queue<int> q;
  int i, v, w;

  fill(used, used+MAX_NODES, 0);
  q.push(src);
  used[src] = 1;

  for (i = 0; i < MAX_NODES; i++) {
    D[i] = -1;
    P[i] = -1;
  }
  D[src] = 0;

  while (!q.empty()) {
    v = q.front();
    q.pop();
    for (i = 0; i < graph[v].deg; i++) {
      w = graph[v].adj[i];
      if (!used[w]) {
	D[w] = D[v] + 1;
	P[w] = v;
	q.push(w);
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

void clear(Node graph[], int n)
{
  int i;
  for (i = 0; i < n; i++) {
    graph[i].deg = 0;
  }
}

void add_edge(Node graph[], int v, int w, int cost)
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

int get_path(int v, int P[], int path[])
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

  clear(graph, MAX_NODES);
  while (cin >> v >> w && v >= 0 && w >= 0) {
    add_edge(graph, v, w, 1);
  }
  
  for (i = 0; i < MAX_NODES; i++) {
    BFS_shortest_path(graph, MAX_NODES, i, D[i], P[i]);
  }

  while (cin >> v >> w && v >= 0 && w >= 0) {
    cout << v << " " << w << ": " << D[v][w] << endl;
    num = get_path(w, P[v], path);
    assert(D[v][w] == -1 || num == D[v][w]+1);
    for (i = 0; i < num; i++) {
      cout << " " << path[i];
    }
    cout << endl;
  }
  return 0;

}
