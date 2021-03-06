// This is the official U of L CodeRunner submission.

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <cassert>
#include <string>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <tr1/tuple>
#include <numeric>
#include <queue>
#include <stack>
using namespace std;
using namespace std::tr1;

#include "util.h"

typedef vector<double> vd;
typedef vd (*Heuristic)(const vector<bool>&);

#include "heuristics.h"

//#define DEBUG

const string default_alpha_file_name = "alpha";

cmd do_turn(const vector<double>& alpha, const vector<Heuristic>& H);
vector<double> read_alpha(int argc,char* argv[]);

vector<Heuristic> fill_heuristics(){
  vector<Heuristic> H;
  
  H.push_back(closest_gold);
  H.push_back(closest_gold_in_scc);
  H.push_back(dig_asap);
  H.push_back(visit_unvisited);
  H.push_back(kill_enemy);
  H.push_back(avoid_enemy);
  H.push_back(avoid_falling_death);
  H.push_back(kill_opp);
  H.push_back(chase_opp);
  H.push_back(kill_yourself);
  H.push_back(randomness);
  H.push_back(IHateDeathAndZombies);
  
  return H;
}

int main(int argc,char* argv[]){
  vector<double> alpha = read_alpha(argc,argv);
  vector<Heuristic> H = fill_heuristics();

  if(alpha.size() != H.size()){
    cerr << "Alpha is a different size than H" << endl;
    return 1;
  }
  
  srand( time(NULL) );
  read_world();
  
  for(CURRENT_TURN=0;CURRENT_TURN<NUMBER_OF_TURNS;CURRENT_TURN++){
    read_turn();

#ifdef DEBUG
    output_command2( do_turn(alpha,H) );
#else
    output_command( do_turn(alpha,H) );
#endif
  }
  return 0;
}

cmd do_turn(const vector<double>& alpha, const vector<Heuristic>& H){
  if(!alive() || falling()) return NONE;
  int n = H.size();

#ifdef DEBUG
  for(int i=0;i<HEIGHT;i++){
    for(int j=0;j<WIDTH;j++)
      cerr << (distance(P1.row,P1.col,i,j) == oo ? -1 : distance(P1.row,P1.col,i,j)) << " ";
    cerr << endl;
  }
#endif
  
  vector<cmd> moves = valid_moves();
  sort(moves.begin(),moves.end());
  vector<bool> vMoves(7,false);
  for(size_t i=0;i<moves.size();i++) vMoves[moves[i]] = true;
  
  vector<double> vals(7,0.0);
  for(int i=0;i<n;i++){
    vector<double> w = H[i](vMoves); // weights
#ifdef DEBUG
    for(int j=0;j<7;j++) cerr << w[j] << " ";
    cerr << endl;
#endif
    for(int j=0;j<7;j++)
      if(vMoves[j])
	vals[j] += alpha[i] * w[j];
  }

#ifdef DEBUG
  cerr << endl;
#endif
  
  for(int i=0;i<7;i++)
    if(!vMoves[i])
      vals[i] = vals[moves[0]] - 1; // Make it worse

  int best_i = moves[0];
  for(int i=6;i>=0;i--)
    if((vMoves[i] && vals[i] > vals[best_i]) ||
       (vMoves[i] && vals[i] == vals[best_i] && rand() % 2))
      best_i = i;
  
  return cmd(best_i);
}

vector<double> read_alpha(int argc,char* argv[]){
  string fileName;
  if(argc > 1) fileName = string(argv[1]);
  else fileName = default_alpha_file_name;

  fstream fin;
  fin.open(fileName.c_str());
  if(!fin.is_open()){
    cerr << "Cannot open file -- Exiting." << endl;
    cerr << "Expecting: " << fileName << endl;
    exit(1);
  }

  vector<double> alpha;
  double tmp;
  while(fin >> tmp) alpha.push_back(tmp);

  return alpha;
}
