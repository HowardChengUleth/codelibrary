/*
 * Finding an Eulerian Tour
 *
 * Author: Howard Cheng
 *
 * The routine eulerian() determines if a graph has an Eulerian tour.
 * That is, it checks that it is connected and all vertices have even
 * degree.  We assume that the graph is represented as an adjacency matrix
 * and the an auxillary array called "deg" gives the degree of the vertex.
 *
 * The routine eulerian_tour() returns one (arbitrary) Eulerian tour.
 * The tour is stored in an array of the vertices visited in the tour,
 * and the first and last vertex is the same.
 *
 * WARNING: eulerian_tour() destroys the graph as it uses edges.  If
 *          you need the graph back then you should save a copy.
 *
 * NOTE: converting this code for directed graphs should not be that much
 *       work.  You should also be able to convert this code for Eulerian
 *       paths.
 *
 */

#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

const int NUM_VERTICES = 50;
const int NUM_EDGES = 1000;       /* maximum number of edges in graph */

int graph[NUM_VERTICES+1][NUM_VERTICES+1];
int deg[NUM_VERTICES+1];

void clear_graph(void)
{
  fill(deg, deg+NUM_VERTICES+1, 0);
  for (int i = 1; i <= NUM_VERTICES; i++) {
    fill(graph[i], graph[i]+NUM_VERTICES+1, 0);
  }
}

void visit(int v, char visited[])
{
  int w;
  
  visited[v] = 1;
  for (w = 1; w <= NUM_VERTICES; w++) {
    if (!visited[w] && graph[v][w] > 0) {
      visit(w, visited);
    }
  }
}

int connected(void)
{
  char visited[NUM_VERTICES+1];
  int i;

  fill(visited, visited+NUM_VERTICES+1, 0);
  for (i = 1; i <= NUM_VERTICES; i++) {
    if (deg[i] > 0) {
      visit(i, visited);
      break;
    }
  }
  for (i = 1; i <= NUM_VERTICES; i++) {
    if (deg[i] > 0 && !visited[i]) {
      return 0;
    }
  }
  return 1;
}

int eulerian(void)
{
  int i;
  for (i = 1; i <= NUM_VERTICES; i++) {
    if (deg[i] % 2 == 1) {
      return 0;
    }
  }
  return connected();
}

int find_tour(int start, int temp[])
{
  int len = 0;
  int next;

  temp[len++] = start;
  while (deg[start] > 0) {
    for (next = 1; next <= NUM_VERTICES; next++) {
      if (graph[start][next] > 0) {
	break;
      }
    }
    temp[len++] = next;
    graph[start][next]--; deg[start]--;
    graph[next][start]--; deg[next]--;
    start = next;
  }
  return len;
}

int graft_tour(int old[], int old_len, int tour[], int tour_len)
{
  int pos[NUM_VERTICES+1];
  int i, j, p1, p2;

  fill(pos, pos+NUM_VERTICES+1, -1);
  for (i = 0; i < old_len; i++) {
    pos[old[i]] = i;
  }
  for (i = 0; i < tour_len; i++) {
    if (pos[tour[i]] >= 0) {
      break;
    }
  }
  assert(i < tour_len);
  p1 = pos[tour[i]];
  p2 = i;
  for (i = old_len-1; i > p1; i--) {
    old[i+tour_len-1] = old[i];
  }
  for (i = p2+1, j = 0; i < tour_len-1; i++, j++) {
    old[p1+j+1] = tour[i];
  }
  for (i = 0; i <= p2; i++) {
    old[p1+j+1] = tour[i];
  }

  return old_len+tour_len-1;
}

int eulerian_tour(int tour[])
{
  int temp[NUM_EDGES+1];
  int tour_len, temp_len, first_time;
  int i, found;

  tour_len = temp_len = 0;
  first_time = 1;

  while (1) {
    found = 0;
    if (first_time) {
      for (i = 1; i <= NUM_VERTICES; i++) {
	if (deg[i] > 0) {
	  found = 1;
	  break;
	}
      }
    } else {
      /* this ensures that we can graft next tour on to existing one */
      for (i = 0; i < tour_len; i++) {
	if (deg[tour[i]] > 0) {
	  found = 1;
	  break;
	}
      }
      i = tour[i];
    }
    if (!found) {
      break;
    }

    if (first_time) {
      tour_len = find_tour(i, tour);
    } else {
      temp_len = find_tour(i, temp);
      tour_len = graft_tour(tour, tour_len, temp, temp_len);
    }
    first_time = 0;
  }
  return tour_len;
}

int main(void)
{
  int T, N, i, j, k;
  int u, v;
  int tour[NUM_EDGES+1], tour_len;
  
  cin >> T;
  for (i = 1; i <= T; i++) {
    clear_graph();
    if (i > 1) {
      cout << endl;
    }
    cout << "Case #" << i << endl;
    cin >> N;
    for (j = 0; j < N; j++) {
      cin >> u >> v;
      graph[u][v]++;
      graph[v][u]++;
      deg[u]++;
      deg[v]++;
    }

    if (eulerian()) {
      tour_len = eulerian_tour(tour);
      for (k = 0; k < tour_len-1; k++) {
	cout << tour[k] << " " << tour[k+1] << endl;
      }
    } else {
      cout << "some beads may be lost" << endl;
    }
  }
  return 0;
}
