/*
 * Topological sort
 *
 * Author: Howard Cheng
 *
 * Given a directed acyclic graph, the topological_sort routine 
 * returns a vector of integers that gives the vertex number (0 to n-1)
 * such that if there is a path from v1 to v2, then v1 occurs earlier
 * than v2 in the order.  Note that the topological sort result is not
 * necessarily unique.
 *
 * topological_sort returns true if there is no cycle.  Otherwise it
 * returns false and the sorting is unsuccessful.
 *
 * The complexity is O(n + m).
 *
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

typedef int Edge;
typedef vector<Edge>::iterator EdgeIter;

struct Graph {
  vector<Edge> *nbr;
  int num_nodes;
  Graph(int n)
    : num_nodes(n)
  {
    nbr = new vector<Edge>[num_nodes];
  }

  ~Graph()
  {
    delete[] nbr;
  }

  // note: There is no check on duplicate edge, so it is possible to
  // add multiple edges between two vertices
  void add_edge(int u, int v)
  {
    nbr[u].push_back(Edge(v));
  }
};

bool topological_sort(const Graph &G, vector<int> &order)
{
  vector<int> indeg(G.num_nodes);
  fill(indeg.begin(), indeg.end(), 0);
  for (int i = 0; i < G.num_nodes; i++) {
    for (int j = 0; j < G.nbr[i].size(); j++) {
      indeg[G.nbr[i][j]]++;
    }
  }

  // use a priority queue if you want to get a topological sort order
  // with ties broken by lexicographical ordering
  queue<int> q;
  for (int i = 0; i < G.num_nodes; i++) {      
    if (indeg[i] == 0) {
      q.push(i);
    }
  }

  order.clear();
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    order.push_back(v);
    for (int i = 0; i < G.nbr[v].size(); i++) {
      if (--indeg[G.nbr[v][i]] == 0) {
	q.push(G.nbr[v][i]);
      }
    }
  }
   
  return order.size() == G.num_nodes;
}


int main(void)
{
  int n, m;

  while (cin >> n >> m && (n || m)) {
    Graph G(n);
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      G.add_edge(u, v);
    }
    vector<int> order;
    if (topological_sort(G, order)) {
      for (int i = 0; i < n; i++) {
	if (i) cout << ' ';
	cout << order[i];
      }
      cout << endl;
    } else {
      cout << "there is a cycle" << endl;
    }
  }
  return 0;
}
