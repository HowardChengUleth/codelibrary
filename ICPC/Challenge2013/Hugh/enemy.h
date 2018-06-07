#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_ENEMY__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_ENEMY__

class Enemy {
public:
  int row,col,master;
  int respawn_row,respawn_col;
  bool alive;
  vector<cmd> circuit;
  int circuit_idx; // Index within the circuit
  stack<cmd> chase;
  
  void read_and_update(){
    // Determine if this move is part of its circuit or chase.
    cin >> row >> col >> master;
    alive = (row >= 0);
    if(!alive) circuit_idx = 0;
  }
  
  void read_and_init(){
    cin >> row >> col;
    respawn_row = row;
    respawn_col = col;
    string s;
    cin >> s;
    for(size_t i=0;i<s.length();i++){
      switch(s[i]){
        case 'L': circuit.push_back(LEFT); break;
        case 'R': circuit.push_back(RIGHT); break;
        case 'B': circuit.push_back(DOWN); break;
        case 'T': circuit.push_back(UP); break;
      }
    }
    circuit_idx = 0;
    master = -1;
  }
};

#endif
