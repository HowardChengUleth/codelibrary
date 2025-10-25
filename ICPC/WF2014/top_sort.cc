// Topological sort (Directed Only) -- O(n+m)
bool topological_sort(const Graph &G, vector<int> &order) {
  vector<int> indeg(G.num_nodes, 0);
  for (int i = 0; i < G.num_nodes; i++) 
    for (int j = 0; j < G.nbr[i].size(); j++)
      indeg[G.nbr[i][j]]++;

  queue<int> q;  // use priority queue if you want tie-breaking by lex ordering
  for (int i = 0; i < G.num_nodes; i++)
    if (indeg[i] == 0) q.push(i);

  order.clear();
  while (!q.empty()) {
    int v = q.front();    q.pop();
    order.push_back(v);
    for (int i = 0; i < G.nbr[v].size(); i++) 
      if (--indeg[G.nbr[v][i]] == 0) q.push(G.nbr[v][i]);
  }
  return order.size() == G.num_nodes;
}
