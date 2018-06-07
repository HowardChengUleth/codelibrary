/*
 * Traversing a geometric graph, always following the most counter-clockwise
 * turn.
 *
 * Author: Howard Cheng
 * Reference:
 *   This is the solution to ACM 1993, Problem B.
 *
 * This is a sample program that takes a graph of points (x,y), and
 * traverses the graph.  The next_node() function finds the start of
 * the traversal (one of the extreme points), and traverse() always chooses
 * the node that gives the most counter-clockwise route.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define MAX_EDGES 200
#define MAX_NODES MAX_EDGES*2

typedef struct {
   int x, y;
} Point;

// counterclockwise, clockwise, or undefined
enum {CCW, CW, CNEITHER};

int num_edges;
int num_nodes;
int graph[MAX_NODES][MAX_NODES];
           // 1 = edge, 0 = no edge, -1 = boundary edge
Point node[MAX_NODES];
int degree[MAX_NODES];
int freq[MAX_EDGES+1];
char visited[MAX_NODES];
int minx, miny, maxx, maxy;
double pi;
FILE *in, *out;

int min(int x, int y)
{
  return (x < y) ? x : y;
}

int max(int x, int y)
{
  return (x > y) ? x : y;
}

int ccw(Point a, Point b, Point c)
{
   double dx1 = b.x - a.x;
   double dx2 = c.x - b.x;
   double dy1 = b.y - a.y;
   double dy2 = c.y - b.y;
   double t1 = dy2 * dx1;
   double t2 = dy1 * dx2;

   if (t1 == t2) {
      if (dx1 * dx2 < 0 || dy1 * dy2 < 0) {
         if (dx1*dx1 + dy1*dy1 >= dx2*dx2 + dy2*dy2) {
            return CNEITHER;
         } else {
            return CW;
         }
      } else {
         return CCW;
      }
   } else if (t1 > t2) {
      return CCW;
   } else {
      return CW;
   }
}

void clear_all(void)
{
   int i, j;
   for (i = 0; i < MAX_NODES; i++) {
      degree[i] = 0;
      for (j = 0; j < MAX_NODES; j++) {
         graph[i][j] = 0;
      }
   }
   num_nodes = 0;
   for (i = 0; i < MAX_EDGES; i++) {
      freq[i] = 0;
   }
}

int node_index(Point p)
{
   int i;
   for (i = 0; i < num_nodes; i++) {
      if (node[i].x == p.x && node[i].y == p.y) {
         return i;
      }
   }

   node[num_nodes++] = p;
   return num_nodes-1;
}

int on_boundary(Point p1, Point p2)
{
   if (p1.x == p2.x) {
      // vertical line
      return (p1.x == minx || p1.x == maxx);
   } else if (p1.y == p2.y) {
      // horizontal line
      return (p1.y == miny || p1.y == maxy);
   }
   return 0;
}

void build_graph(void)
{
   Point p1, p2;
   int i, j, n1, n2;

   minx = miny = 10001;
   maxx = maxy = 0;

   for (i = 0; i < num_edges; i++) {
      fscanf(in, "%d %d %d %d\n", &p1.x, &p1.y, &p2.x, &p2.y);
      minx = min(minx, min(p1.x, p2.x));
      maxx = max(maxx, max(p1.x, p2.x));
      miny = min(miny, min(p1.y, p2.y));
      maxy = max(maxy, max(p1.y, p2.y));
      n1 = node_index(p1);
      n2 = node_index(p2);
      graph[n1][n2] = graph[n2][n1] = 1;
      degree[n1]++;
      degree[n2]++;
   }

   // find boundary
   for (i = 0; i < num_nodes; i++) {
      for (j = i+1; j < num_nodes; j++) {
         if (graph[i][j] == 1 && on_boundary(node[i], node[j])) {
            graph[i][j] = graph[j][i] = -1;
         }
      }
   }
}

int next_node(int *start)
{
   int best_init;
   int i, v;
   int c, a[2], res;

   for (i = 0; i < num_nodes; i++) {
      visited[i] = 0;
   }

   // first find the one with the lowest x and then lowest y coordinates
   best_init = 0;
   for (i = 0; i < num_nodes; i++) {
      if (degree[i] > 0) {
         if (!best_init || node[i].x < node[v].x ||
             (node[i].x == node[v].x && node[i].y < node[v].y)) {
            v = i;
            best_init = 1;
         }
      }
   }
   if (!best_init) {
      return -1;      // no possible vertex
   }

   // get the next two vertices adjacent to v by a boundary
   c = 0;
   for (i = 0; i < num_nodes; i++) {
      if (graph[v][i] == -1) {
         if (c < 2) {
            a[c++] = i;
         } else {
            fprintf(stderr, "More than 2 boundary vertices adjacent to v\n");
            assert(0);
         }
      }
   }

   assert(c == 2);
   c = ccw(node[v], node[a[0]], node[a[1]]);
   if (c == CCW) {
      res = a[0];
   } else {
      assert(c == CW);
      res = a[1];
   }

   assert(graph[v][res] == -1);
   graph[v][res] = graph[res][v] = 0;
   degree[v]--;
   degree[res]--;

   *start = v;
   return res;

}

void traverse(int v, int edges, int start, int prev)
{
   double a1, a2, best_ang;
   int i, w, best_init = 0;

   if (v == start) {
      freq[edges]++;
      return;
   }

   visited[v] = 1;

   /* now find the edge that goes as CCW as possible */
   a1 = atan2(node[prev].y-node[v].y, node[prev].x-node[v].x);
   for (i = 0; i < num_nodes; i++) {
      if (graph[v][i] && !visited[i]) {
         a2 = atan2(node[i].y-node[v].y, node[i].x-node[v].x) - a1;
         while (a2 < 0.0) {
            a2 += 2*pi;
         }
         if (!best_init || a2 > best_ang) {
            best_init = 1;
            best_ang = a2;
            w = i;
         }
      }
   }

   if (best_init) {
      /* found it */
      if (graph[v][w] == 1) {
         graph[v][w] = graph[w][v] = -1;
      } else {
         assert(graph[v][w] == -1);
         graph[v][w] = graph[w][v] = 0;
         degree[v]--;
         degree[w]--;
      }
      traverse(w, edges+1, start, v);
   }
}

void print_freq(void)
{
   int sum = 0;
   int i;

   for (i = 0; i < MAX_EDGES; i++) {
      if (freq[i] > 0) {
         sum += freq[i];
         fprintf(out, "Number of lots with perimeter consisting of %d "
                 "surveyor's lines = %d\n", i, freq[i]);
      }
   }
   fprintf(out, "Total number of lots = %d\n", sum);
}

int main(void)
{
   int count, v, i, j, start;
   in = fopen("lots.in", "r");
   out = fopen("lots.out", "w");
   assert(in && out);
   setbuf(out, 0);

   pi = atan(1)*4;
   count = 0;
   while (fscanf(in, "%d\n", &num_edges) == 1 && num_edges > 0) {
      if (count > 0) {
         fprintf(out, "\n");
      }
      fprintf(out, "Case %d\n", ++count);
      clear_all();
      build_graph();
      while ((v = next_node(&start)) != -1) {
         traverse(v, 1, start, start);

#ifdef DEBUG
         fprintf(out, "\nGraph:\n\n");
         for (i = 0; i < num_nodes; i++) {
            for (j = 0; j < num_nodes; j++) {
               fprintf(out, "%3d", graph[i][j]);
            }
            fprintf(out, "\n");
         }
         fprintf(out, "\n");
#endif
      }
      print_freq();
   }

   return 0;
}
