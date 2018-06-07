// This is a template for Coderunner program for the U of L.

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
using namespace std;

#include "/home/cheng/Challenge/Darcy/util.h"

// This is where the magic happens!
void do_turn(){
  int r,c;
  curr_loc(r,c);
  
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
