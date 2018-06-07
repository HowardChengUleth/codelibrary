#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_ENEMY__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_ENEMY__

// I commented out a lot of code... If you want it back, feel free.

// Time before this enemy respawns
const int ENEMY_RESPAWN_TIME = 25;
vector<int> enemy_respawns_in;

class Enemy {
public:
  int row,col,master,id;
  int respawn_row,respawn_col;
  bool alive;
  // vector<cmd> circuit;
  //int circuit_idx; // Index within the circuit
  //stack<cmd> chase;
  
  void read_and_update(){
    cin >> row >> col >> master;
    alive = (row >= 0);

    if(alive){
      enemy_respawns_in[id] = -1;
    } else {
      if(enemy_respawns_in[id] == -1)
	enemy_respawns_in[id] = ENEMY_RESPAWN_TIME;
      else
	enemy_respawns_in[id]--;
    }
  }
  
  void read_and_init(int ID){
    cin >> row >> col;

    respawn_row = row;
    respawn_col = col;
    string s;
    cin >> s;

    id = ID;
    enemy_respawns_in.push_back(-1);
/*
    for(size_t i=0;i<s.length();i++){
      switch(s[i]){
        case 'L': circuit.push_back(LEFT); break;
        case 'R': circuit.push_back(RIGHT); break;
        case 'B': circuit.push_back(DOWN); break;
        case 'T': circuit.push_back(UP); break;
      }
    }
    circuit_idx = 0;
*/
    master = -1;
  }
};

#endif
