#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <cassert>
using namespace std;

map<string,int> nodes;
map<int,string> cities;
map<int,string> hiways;
vector<int> queries;
int n;

const int MAX_N = 105, DISCONNECT = -1;
int graph[MAX_N][MAX_N], dist[MAX_N][MAX_N], succ[MAX_N][MAX_N];

void readMap() {
  queries.clear(); nodes.clear(); cities.clear(); hiways.clear();
  for (int i = 0; i < MAX_N; ++i) fill(graph[i], graph[i]+MAX_N, DISCONNECT);
  for (int i = 0; i < MAX_N; ++i) graph[i][i] = 0;
  
  string buf; n = 0;
  while (getline(cin,buf) && buf != "") {
    vector<string> fields;
    string tmp;
    for (int i = 0; i < buf.size(); ++i) {
      if (buf[i] == ',') { fields.push_back(tmp); tmp = ""; }
      else tmp += buf[i];
    }
    fields.push_back(tmp);
    assert(fields.size() == 4);

    int d, c1, c2;
    istringstream iss(fields[3]); iss >> d;
    if (nodes.find(fields[0]) == nodes.end()) nodes[fields[0]] = n++;
    if (nodes.find(fields[1]) == nodes.end()) nodes[fields[1]] = n++;
    c1 = nodes[fields[0]], c2 = nodes[fields[1]];
    cities[c1] = fields[0], cities[c2] = fields[1];
    if (graph[c1][c2] == DISCONNECT || graph[c1][c2] > d) {
      hiways[c1 + (c2*110)] = fields[2];
      hiways[c2 + (c1*110)] = fields[2];
      graph[c1][c2] = d; graph[c2][c1] = d;
    }
  }
  while (getline(cin,buf) && buf != "") {
    vector<string> fields;
    string tmp;
    for (int i = 0; i < buf.size(); ++i) {
      if (buf[i] == ',') { fields.push_back(tmp); tmp = ""; }
      else tmp += buf[i];
    }
    fields.push_back(tmp);
    assert(fields.size() == 2);
    queries.push_back(nodes[fields[0]] + (nodes[fields[1]]*110));
  }
}

void floyd(int n) {
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      dist[i][j] = graph[i][j];
      succ[i][j] = (i==j || graph[i][j] == DISCONNECT) ? -1 : j;
    }
  for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
	if (i != k && dist[i][k] != DISCONNECT && dist[k][j] != DISCONNECT) {
	  int temp = dist[i][k] + dist[k][j];
	  if (dist[i][j] == DISCONNECT || dist[i][j] > temp) {
	    dist[i][j] = temp; succ[i][j] = succ[i][k];
	  } else if (dist[i][j] == temp && succ[i][k] < succ[i][j]) {
	    succ[i][j] = succ[i][k];
	  }
	}
  for (int i = 0; i < n; ++i) dist[i][i] = 0;
}

int extract_path(int u, int v, int path[]) {
  int len = 0;
  if (dist[u][v] == DISCONNECT) return -1;
  path[len++] = u;
  while (u != v) {
    u = succ[u][v]; path[len++] = u;
  }
  return len;
}

void print(int q) {
  cout << endl << endl;
  cout << "From                 To                   Route      Miles\n";
  cout << "-------------------- -------------------- ---------- -----\n";
  int path[MAX_N*MAX_N];
  int len = extract_path(q%110, q/110, path);
  for (int i = 0; i < len-1; ++i) {
    cout << left << setw(20) << cities[path[i]] << " "
	 << left << setw(20) << cities[path[i+1]] << " "
	 << left << setw(10) << hiways[path[i] + (path[i+1]*110)] << " "
	 << right << setw(5) << graph[path[i]][path[i+1]] << endl;
  }
  cout << "                                                     -----\n";
  cout << "                                          Total      "
       << right << setw(5) << dist[q%110][q/110] << endl;
}

int main() {
  readMap();
  floyd(n);
  for (int i = 0; i < queries.size(); ++i)
    print(queries[i]);
  return 0;
}
  
