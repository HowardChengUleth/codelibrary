// Eulerian Tour (Undirected or Directed) -- O(mn) [Change to adj list --> O(m+n)]
//  -- Returns one arbitrary Eulerian tour: destroys original graph!
// To run: tour.clear(), then call find_tour on any vertex with a non-zero degree
//
// If there are self loops, make sure graph[u][u] is incremented twice.
//
// FACTS:
// 1. Undirected G has CLOSED Eulerian <--> (G connected) && (every vertex has
//    even degree)
// 2. Directed G has CLOSED Eulerian <--> (G strongly connected) &&
//    (in-degree==out-degree)
// 3. G has an OPEN Eulerian <--> All but two vertices satisfy the right
//    condition above, and adding an edge between them satisfies both conditions.

int graph[MAX_N][MAX_N];

vector<int> tour;
void find_tour(int u,int n){ // n is the number of vertices
  for(int v=0;v<n;v++)
    while(graph[u][v]){
      graph[u][v]--;
      graph[v][u]--;       // this line is only for undirected graphs!!!
      find_tour(v,n);
    }
  tour.push_back(u);
}
