/*
 * Network Flow
 *
 * Author: Howard Cheng
 *
 * The routine network_flow() finds the maximum flow that can be
 * pushed from the source (s) to the sink (t) in a flow network
 * (i.e. directed graph with capacities on the edges).  The maximum
 * flow is returned.  Note that the graph is modified, so that the
 * capacity is decreased by the amount pushed through the edge.  If
 * you wish to recover the flow on an edge, it is max(0,capacity - residual).
 *
 * Note: it is possible for the residual capacities of the resulting graph to
 *       be greater than original capacities.  This happens when a flow is
 *       pushed through an edge, and the "reverse edge" has its residual
 *       capacity increased.  Therefore, make sure you only take the
 *       difference if the difference is positive.
 *
 * Note: it is required that if (u,v) is an edge, then there must be
 *       an edge (v,u).  In the code below add_edge() simply adds
 *       (v,u) with capacity 0.  If the graph actually has edges in
 *       both directions, then use the given capacity instead of 0.
 *
 * This code can also be used for bipartite matching by setting up an
 * appropriate flow network.
 *
 * The code here assumes an adjacency list representation since most
 * problems requiring network flow have sparse graphs.
 *
 * This is the basic augmenting path algorithm and it is not the most
 * efficient.  But it should be good enough for most programming contest
 * problems.  The complexity is O(f m) where f is the size of the flow
 * and m is the number of edges.  This is good if you know that f
 * is small, but can be exponential if f is large.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_NODE 100
#define MAX_DEG 100

typedef struct {
    int deg;
    int nbrs[MAX_DEG], cap[MAX_DEG], res[MAX_DEG];
} Node;

void clear_graph(Node *graph)
{
  int i;

  for (i = 0; i < MAX_NODE; i++) {
    graph[i].deg = 0;
  }
}

void add_edge(Node *graph, int u, int v, int cap)
{
  graph[u].nbrs[graph[u].deg] = v;
  graph[u].cap[graph[u].deg] = cap;
  graph[u].res[graph[u].deg++] = cap;

  /* WARNING: see note in comment at the top */
  graph[v].nbrs[graph[v].deg] = u;
  graph[v].cap[graph[v].deg] = 0;
  graph[v].res[graph[v].deg++] = 0;
}

/* the path is stored in a peculiar way for efficiency:

   Let v(i) be the index of the vertex in the i-th element of the path.
   Then v(i) is computed by graph[v(i-1)].nbrs[path[i]], where
   v(-1) = source vertex.
*/
   
int augmenting_path(Node *graph, int s, int t, int *path, char *visited,
		    int minflow)
{
  int i, v, flow;
  
  if (s == t) {
    return -1;
  }
  for (i = 0; i < graph[s].deg; i++) {
    v = graph[s].nbrs[i];
    if (graph[s].res[i] >= minflow && !visited[v]) {
      path[0] = i;
      visited[v] = 1;
      if ((flow = augmenting_path(graph, v, t, path+1, visited, minflow))) {
	if (flow == -1) {
	  return graph[s].res[i];
	} else {
	  return (flow < graph[s].res[i]) ? flow : graph[s].res[i];
	}
      }
    }
  }
  return 0;
}

void push_path(Node *graph, int s, int t, int *path, int flow)
{
  int v, w;
  int i, j;

  for (v = s, i = 0; v != t; v = w, i++) {
    w = graph[v].nbrs[path[i]];
    graph[v].res[path[i]] -= flow;
    for (j = 0; j < graph[w].deg; j++) {
      if (graph[w].nbrs[j] == v) {
	graph[w].res[j] += flow;
	break;
      }
    }
  }
}

int network_flow(Node *graph, int s, int t)
{
  char visited[MAX_NODE];
  int path[MAX_NODE];
  int flow = 0, f = 1;
  int i;

  while (f > 0) {
    memset(visited, 0, MAX_NODE);
    if ((f = augmenting_path(graph, s, t, path, visited, 1)) > 0) {
      push_path(graph, s, t, path, f);
      flow += f;
    }
  }

  return flow;
}

int main(void)
{
  Node graph[MAX_NODE];
  int s, t, u, v, cap, flow;

  clear_graph(graph);
  scanf("%d %d", &s, &t);
  while (scanf("%d %d %d", &u, &v, &cap) == 3) {
    add_edge(graph, u, v, cap);
  }
  
  flow = network_flow(graph, s, t);
  printf("maximum flow = %d\n", flow);
  
  return 0;
}


