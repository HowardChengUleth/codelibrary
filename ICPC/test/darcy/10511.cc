#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

using namespace std;

#include <iostream>
#include <vector>
using namespace std;

struct Edge;
typedef vector<Edge>::iterator EdgeIter;

struct Edge {
  int to, cap, flow;
  bool is_real;
  pair<int,int> part;
  EdgeIter partner;
  
  int residual() const { return cap - flow; }
};

struct Graph {
  vector<vector<Edge> > nbr;
  int num_nodes;
  Graph(int n) : nbr(n), num_nodes(n) { }

  void add_edge_directed(int u,int v,int cap,int flow,bool is_real,pair<int,int> part){
    Edge e = {v,cap,flow,is_real,part}; nbr[u].push_back(e);
  }
};


// Use this instead of G.add_edge_directed in your actual program
void add_edge(Graph& G,int u,int v,int cap){
  int U = G.nbr[u].size(), V = G.nbr[v].size();
  G.add_edge_directed(u,v,cap,0,true ,make_pair(v,V));
  G.add_edge_directed(v,u,0  ,0,false,make_pair(u,U));
}

void push_path(Graph& G, int s, int t, const vector<EdgeIter>& path, int flow) {
  for (int i = 0; s != t; s = path[i++]->to)
    if (path[i]->is_real) {
      path[i]->flow += flow;    path[i]->partner->cap += flow;
    } else {
      path[i]->cap -= flow;     path[i]->partner->flow -= flow;
    }
}

int augmenting_path(Graph& G, int s, int t, vector<EdgeIter>& path,
		    vector<bool>& visited, int step = 0) {
  if (s == t) return -1;  visited[s] = true;
  for (EdgeIter it = G.nbr[s].begin(); it != G.nbr[s].end(); ++it) {
    int v = it->to;
    if (it->residual() > 0 && !visited[v]) {
      path[step] = it;
      int flow = augmenting_path(G, v, t, path, visited, step+1);
      if (flow == -1)    return it->residual();
      else if (flow > 0) return min(flow, it->residual());
    }
  }
  return 0;
}

int network_flow(Graph& G, int s, int t) { // note that the graph is modified
  for(int i=0;i<G.num_nodes;i++)
    for(EdgeIter it=G.nbr[i].begin(); it != G.nbr[i].end(); ++it)
      G.nbr[it->part.first][it->part.second].partner = it;
  
  vector<EdgeIter> path(G.num_nodes);
  int flow = 0, f;
  do {
    vector<bool> visited(G.num_nodes, false);
    if ((f = augmenting_path(G, s, t, path, visited)) > 0) {
      push_path(G, s, t, path, f);    flow += f;
    }
  } while (f > 0);
  return flow;
}


#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

const int MAX_PEOPLE = 1001;
const int MAX_CLUBS = 44*MAX_PEOPLE;
const int MAX_PARTIES = MAX_PEOPLE;
const int SOURCE = MAX_PEOPLE + MAX_CLUBS + MAX_PARTIES;
const int SINK = SOURCE + 1;

#define PERSON(X) (X)
#define PARTY(X) (MAX_PEOPLE + (X))
#define CLUB(X) (MAX_PEOPLE + MAX_PARTIES + (X))

int numPeople,numParties,numClubs;
string personName[MAX_PEOPLE],clubName[MAX_CLUBS];
map<string,int> people,parties,clubs;

int num(map<string,int>& M,const string& s,int& num){
  if(M.find(s) == M.end())
    M[s] = num++;
  return M[s];
}

void do_case(){
  Graph G(SINK+1);
  people.clear();
  parties.clear();
  clubs.clear();
  numPeople = numParties = numClubs = 0;
	
  string person,party,club;
	
  string s;
  while(getline(cin,s) && s != ""){
    stringstream ss;
    ss.str(s);
    ss >> person >> party;
    personName[num(people,person,numPeople)] = person;
    add_edge(G,PERSON(num(people,person,numPeople)),PARTY(num(parties,party,numParties)),1);
    while(ss >> club){
      clubName[num(clubs,club,numClubs)] = club;
      add_edge(G,CLUB(num(clubs,club,numClubs)),PERSON(num(people,person,numPeople)),1);
    }
  }
  for(int i=0;i<numClubs;i++)
    add_edge(G,SOURCE,CLUB(i),1);
	
  for(int i=0;i<numParties;i++)
    add_edge(G,PARTY(i),SINK,(numClubs+1)/2-1);
	
  int flow = network_flow(G, SOURCE, SINK);
  if(flow != numClubs){
    cout << "Impossible." << endl;
  } else {
    for(int i=CLUB(0);i<CLUB(numClubs);i++)
      for(EdgeIter it=G.nbr[i].begin();
	  it!=G.nbr[i].end();++it)
	if(it->flow)
	  cout << personName[it->to] << " " << clubName[i-CLUB(0)] << endl;
  }
}

int main(){
  int T;
  cin >> T;
  string s;
  getline(cin,s);
  getline(cin,s);
  while(T--){
    do_case();
    if(T)
      cout << endl;
  }
  return 0;
}
