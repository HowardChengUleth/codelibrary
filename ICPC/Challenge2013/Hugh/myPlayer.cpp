#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
using namespace std;

#include "/home/cheng/Challenge/Darcy/util.h"

typedef vector<double> (*Heuristic)();

// This is where the magic happens!
void do_turn(const vector<double>& alpha, const vector<Heuristic>& H){
   vector<cmd> valid = valid_moves();

   int n = H.size();
   
   vector<double> vals(7,0.0);
   for(int i=0;i<n;i++){
      vector<double> tmp = H[i].second();
      for(int j=0;j<7;j++)
	 vals += H[i].first * 
   }
}
  int r,c;
  curr_loc(r,c);
  if(can_hammer())
     
  pair<cmd,int> t = closest_gold();
  output_command ( t.first );
}

int main(){
  // You only need this line if there is randomness in your program
  srand( time(NULL) );

  // Read in the world
  read_world();

  for(CURRENT_TURN=0;CURRENT_TURN<NUMBER_OF_TURNS;CURRENT_TURN++){
    read_turn();
    do_turn();
  }
  return 0;
}
