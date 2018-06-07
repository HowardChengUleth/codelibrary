/*
 * Implementation of Kruskal's Minimum Spanning Tree Algorithm
 *
 * Author: Howard Cheng
 *
 * This is a routine to find the minimum spanning tree.  It takes as
 * input:
 *
 *      n: number of vertices
 *      m: number of edges
 *  elist: an array of edges (if (u,v) is in the list, there is no need
 *         for (v,u) to be in, but it wouldn't hurt, as long as the weights
 *         are the same).
 *
 * The following are returned:
 *
 *  index: an array of indices that shows which edges from elist are in
 *         the minimum spanning tree.  It is assumed that its size is at
 *         least n-1.
 *   size: the number of edges selected in "index".  If this is not
 *         n-1, the graph is not connected and we have a "minimum
 *         spanning forest."
 *
 * The weight of the MST is returned as the function return value.
 * 
 * The run time of the algorithm is O(m log m).
 *
 * Note: the elements of elist may be reordered.
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_N          110
#define MAX_M          MAX_N*(MAX_N)

typedef struct {
  int p, rank;
} UnionFind;

void init_uf(UnionFind *uf, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    uf[i].p = i;
    uf[i].rank = 0;
  }
}

int find(UnionFind *uf, int n, int x)
{
  if (uf[x].p != x) {
    uf[x].p = find(uf, n, uf[x].p);
  }
  return uf[x].p;
}

void merge(UnionFind *uf, int n, int x, int y)
{
  int res1, res2;

  res1 = find(uf, n, x);
  res2 = find(uf, n, y);
  if (res1 != res2) {
    if (uf[res1].rank > uf[res2].rank) {
      uf[res2].p = res1;
    } else {
      uf[res1].p = res2;
      if (uf[res1].rank == uf[res2].rank) {
	uf[res2].rank++;
      }
    }
  }
}

typedef struct {
  int v1, v2;          /* two endpoints of edge                */
  double w;               /* weight, can be double instead of int */
} Edge;

int edge_cmp(const void *a, const void *b)
{
  Edge *e1 = (Edge *)a, *e2 = (Edge *)b;
  if (e1->w < e2->w) {
    return -1;
  } else if (e1->w > e2->w) {
    return 1;
  } else {
    return 0;
  }
}

double mst(int n, int m, Edge *elist, int *index, int *size)
{
  int i, c1, c2;
  double w;
  UnionFind uf[MAX_N];

  qsort(elist, m, sizeof(*elist), edge_cmp);

  init_uf(uf, n);
  w = 0;
  *size = 0;
  for (i = 0; i < m && *size < n-1; i++) {
    c1 = find(uf, n, elist[i].v1);
    c2 = find(uf, n, elist[i].v2);
    if (c1 != c2) {
      index[(*size)++] = i;
      w += elist[i].w;
      merge(uf, n, c1, c2);
    }
  }

  return w;
}

int main(void)
{
  int index[MAX_N];
  double x[MAX_N], y[MAX_N];
  Edge elist[MAX_M];
  int n, m, i, j, k, t;
  double w;

  scanf("%d", &n);
  for (i = 0; i < n; i++) {
    scanf("%lf %lf", x+i, y+i);
  }
  
  k = 0;
  for (i = 0; i < n; i++) {
    for (j = i+1; j < n; j++) {
      elist[k].v1 = i;
      elist[k].v2 = j;
      elist[k].w = hypot((x[i]-x[j]), (y[i]-y[j]));
      k++;
    }
  }

  w = mst(n, k, elist, index, &t);
  printf("%.2f\n", w);
  return 0;
}



