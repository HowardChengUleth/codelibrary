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
#include "globals.h"
#include "classes.h"

typedef vector<double> vd;
typedef vd (*Heuristic)(const vector<bool>&);

#include "heuristics.h"

//#define DEBUG
#define TURN_TIMER

#ifdef TURN_TIMER
vector<double> times;
#endif

const string default_alpha_file_name = "alpha";
cmd do_turn(const vector<double>& alpha, const vector<Heuristic>& H);
vector<double> read_alpha(int argc,char* argv[]);
vector<Heuristic> fill_heuristics(){
  vector<Heuristic> H;
  
  H.push_back(closest_gold);
  H.push_back(closest_3gold_TSP);
  //  H.push_back(closest_gold_in_scc);
  //  H.push_back(dig_asap);
  H.push_back(visit_unvisited);
  H.push_back(kill_enemy);
  //  H.push_back(avoid_enemy);
  //  H.push_back(avoid_falling_death);
  H.push_back(kill_opp_dist0);
  H.push_back(kill_opp_dist2);
  //  H.push_back(chase_opp);
  H.push_back(kill_yourself);
  //  H.push_back(randomness);
  //H.push_back(IHateDeathAndZombies);
  //  H.push_back(gold_trap);
  H.push_back(ClosestGoldLookahead);
  H.push_back(never_run_into_opp_hole);
  H.push_back(no_walk_into_filled_removed_brick);
  H.push_back(no_reverse);
  H.push_back(avoid_stuck);
  H.push_back(kevins_lookahead);
  //  H.push_back(dont_go_into_leaf_scc);
  
  return H;
}

int main(int argc,char* argv[]){
#ifdef TURN_TIMER
  clock_t start_prog = clock();
#endif
  vector<double> alpha = read_alpha(argc,argv);
  vector<Heuristic> H = fill_heuristics();

  if(alpha.size() != H.size()){
    cerr << "Alpha is a different size than H" << endl;
    cerr << "H: " << H.size() << endl;
    cerr << "a: " << alpha.size() << endl;
    return 1;
  }
  
  srand( time(NULL) );

  read_world();

#ifdef TURN_TIMER
  clock_t end_prog = clock();
  cerr << "Initialization   : " << (1000.0*(end_prog - start_prog) / CLOCKS_PER_SEC) << endl;
#endif

  string move = "NONE";
  for(CURRENT_TURN=0;CURRENT_TURN<NUMBER_OF_TURNS;CURRENT_TURN++){
    read_turn(move);
    cmd whatwedo = do_turn(alpha,H);
#ifdef DEBUG
    move = output_command2(whatwedo);
#else
    move = output_command(whatwedo);
#endif
    lastcmd = whatwedo;
    
  }

#ifdef TURN_TIMER
  double maxi = 0,tot = 0;
  for(size_t i=0;i<times.size();i++){
    tot += times[i];
    maxi = max(maxi,times[i]);
  }
  cerr << "Average turn time: " << tot/times.size() << endl;
  cerr << "Longest turn time: " << maxi << endl;
  
#endif

  return 0;
}

cmd do_turn(const vector<double>& alpha, const vector<Heuristic>& H){
  if(!alive() || falling()) return NONE;
#ifdef TURN_TIMER
  clock_t st = clock();
#endif

  int n = H.size();
  
  vector<cmd> moves = valid_moves();
  sort(moves.begin(),moves.end());

  vector<bool> vMoves(7,false);
  for(size_t i=0;i<moves.size();i++) vMoves[moves[i]] = true;

  /*
  for(int i=0;i<7;i++) cerr << (vMoves[i] ? 1 : 0);
  cerr << endl;
  */
  
  if(level_of_suicide() < 2){
    vector<cmd> vv;
    Lookahead::boundedLookaheadABD(se->g,*(se->ourStatus),1,TURNS_REMAINING,5,vv);
    //    cerr << vv.size() << endl;
    vector<bool> curr(7,false);
    for(size_t i=0;i<vv.size();i++)
      curr[vv[i]] = true;
    for(int i=0;i<7;i++)
      if(!curr[i])
	vMoves[i] = false;
  }

    
  int ctr = 0;
  for(int i=0;i<7;i++) if(vMoves[i])  ctr++;
  if(ctr == 0) vMoves[NONE] = true;
  
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
      vals[i] = vals[moves[0]] - 1; 
  
  vector<int> best_i;
  best_i.push_back(moves[0]);
  for(int i=6;i>=0;i--) {
    if(vMoves[i] && vals[i] > vals[best_i[0]]) {
      best_i.clear();
      best_i.push_back(i);
    } else if (vMoves[i] && vals[i] == vals[best_i[0]]) {
      best_i.push_back(i);
    }
  }

  sort(best_i.begin(),best_i.end());
  best_i.resize(unique(best_i.begin(),best_i.end())-best_i.begin());

  
#ifdef TURN_TIMER
  clock_t en = clock();
  times.push_back((1000.0*(en-st))/(CLOCKS_PER_SEC*1.0));
#endif
  if(best_i.size() > 1)
     for(unsigned int i=0;i<best_i.size();i++){
	if(best_i[i] == opp[lastcmd]){
	   swap(best_i[i], best_i[best_i.size()-1]);
	   best_i.pop_back();
	   break;
	}
     }

  return cmd(best_i[rand() % best_i.size()]);
}

vector<double> read_alpha(int argc,char* argv[]){
  string fileName;
  if(argc > 1) fileName = string(argv[1]);
  else fileName = default_alpha_file_name;

  vector<double> alpha;
  fstream fin;
  fin.open(fileName.c_str());
  if(!fin.is_open()){
    cerr << "Cannot open file -- Exiting." << endl;
    cerr << "Expecting: " << fileName << endl;
    exit(1);
  } else {
    double tmp;
    while(fin >> tmp) alpha.push_back(tmp);
  }

  return alpha;
}
