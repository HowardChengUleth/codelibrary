/*
 * Union-find algorithm for equivalence classes
 *
 * Author: Howard Cheng
 *
 * Given n labels (0, 1, ..., n-1), the union and find operations are
 * implemented for equivalence classes computation.
 *
 * Revised: 21 June 2000 by Howard Cheng <hchcheng@scg.math.uwaterloo.ca>
 *   - use weighted union (cf. Cormen, Leiserson, Rivest, Ch. 22) to improve
 *     amortized run time
 */

#include <stdio.h>
#include <stdlib.h>

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

int find(UnionFind *uf, int x)
{
  if (uf[x].p != x) {
    uf[x].p = find(uf, uf[x].p);
  }
  return uf[x].p;
}

void merge(UnionFind *uf, int x, int y)
{
  int res1, res2;

  res1 = find(uf, x);
  res2 = find(uf, y);
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

int main(void)
{
  int n;
  int op, x, y;
  UnionFind *uf;
  int count = 1;

  while (scanf("%d", &n) == 1 && n > 0) {
    printf("Case %d\n", count++);
    uf = malloc(n*sizeof(*uf));
    if (!uf) {
      fprintf(stderr, "out of memory\n");
    }
    init_uf(uf, n);
    while (scanf("%d", &op) == 1 && op != 0) {
      if (op == 1) {
        scanf("%d %d", &x, &y);
        merge(uf, x, y);
	for (x = 0; x < n; x++) {
	  printf("find(%d) = %d\n", x, find(uf, x));
	}
      } else {
        scanf("%d", &x);
        printf("find(%d) = %d\n", x, find(uf, x));
      }
    }
    free(uf);
  }

  return 0;
}
