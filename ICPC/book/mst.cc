// Minimum Spanning Tree (Undirected Only) -- O(m*log(m))
//  -- Do NOT type the Graph structure (not needed)
//  -- Include unionfind code

typedef double Weight;     // can be int instead

struct Edge {
  int v1, v2;          // two endpoints of edge                
  Weight w;     
  Edge(int i=-1, int j=-1, Weight weight=0) :  v1(i), v2(j), w(weight) { }
  bool operator<(const Edge& e) const { return w < e.w; }
};

Weight mst(int n, int m, Edge elist[], int index[], int& size) {
  UnionFind uf(n);
  sort(elist, elist+m);

  Weight w = 0;    size = 0;
  for (int i = 0; i < m && size < n-1; i++) {
    if (uf.merge(elist[i].v1, elist[i].v2)) {
      index[size++] = i;   w += elist[i].w;
    }
  }
  return w;
}
