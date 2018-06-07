#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#include "util.h"

// This is where the magic happens!
void do_turn(){
  vector<cmd> v = valid_moves();
  output_command( v[ rand() % v.size() ] );
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
