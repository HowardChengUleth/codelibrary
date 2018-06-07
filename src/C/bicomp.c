/*
 * Biconnected Components
 *
 * Author: Howard Cheng
 * Date: Oct 15, 2004
 *
 * The routine bicomp() uses DFS to find the biconnected components in
 * a graph.  The graph is stored as an adjacency list.  Use clear_graph()
 * and add_edge() to build the graph.
 *
 * Note: This works only on connected graphs.
 *
 * The code simply prints the biconnected components and the articulation
 * points.  Replace the printing code to do whatever is appropriate.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* maximum number of nodes, maximum degree, and maximum number of edges */
#define MAX_N 1000
#define MAX_DEG 4
#define MAX_M MAX_DEG*MAX_N/2

typedef struct {
  int deg;
  int nbrs[MAX_DEG];
  int dfs, back;
} Node;

int n_stack, dfn;
int v_stack[MAX_M];
int w_stack[MAX_M];

int min(int x, int y)
{
  return (x < y) ? x : y;
}

void clear_graph(Node *G, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    G[i].deg = 0;
  }
}

void add_edge(Node *G, int u, int v)
{
  G[u].nbrs[G[u].deg++] = v;
  G[v].nbrs[G[v].deg++] = u;
}

void do_dfs(Node *G, int v, int pred)
{
  int i, w, child = 0;

  G[v].dfs = G[v].back = ++dfn;
  for (i = 0; i < G[v].deg; i++) {
    w = G[v].nbrs[i];
    if (G[w].dfs < G[v].dfs && w != pred) {
      /* back edge or unexamined forward edge */
      v_stack[n_stack] = v;
      w_stack[n_stack++] = w;
    }
    if (!G[w].dfs) {
      do_dfs(G, w, v);
      child++;

      /* back up from recursion */
      if (G[w].back >= G[v].dfs) {
	/* new bicomponent */
	printf("edges in new biconnected component:\n");
	while (v_stack[n_stack-1] != v || w_stack[n_stack-1] != w) {
	  assert(n_stack > 0);
	  printf("%d %d\n", v_stack[n_stack-1], w_stack[n_stack-1]);
	  n_stack--;
	}
	printf("%d %d\n", v_stack[n_stack-1], w_stack[n_stack-1]);
	n_stack--;
	if (pred != -1) {
	  printf("articulation point: %d\n", v);
	}
      } else {
	G[v].back = min(G[v].back, G[w].back);
      }
    } else {
      /* w has been examined already */
      G[v].back = min(G[v].back, G[w].dfs);
    }
  }
  if (pred == -1 && child > 1) {
    printf("articulation point: %d\n", v);
  }
}

void bicomp(Node *G, int n)
{
  int i;

  n_stack = 0;
  dfn = 0;
  for (i = 0; i < n; i++) {
    G[i].dfs = 0;
  }
  do_dfs(G, 0, -1);
}

int main(void)
{
  Node G[MAX_N];
  int n, m, i, u, v;

  scanf("%d", &n);
  clear_graph(G, n);
  scanf("%d", &m);
  for (i = 0; i < m; i++) {
    scanf("%d %d", &u, &v);
    add_edge(G, u-1, v-1);
  }
  bicomp(G, n);
  return 0;
}
