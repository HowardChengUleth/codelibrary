#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_PLAYER__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_PLAYER__

class Player {
public:
  int row,col,dig_delay,score;
  bool alive;
  int respawn_row,respawn_col;
  
  void read();
  
  void read_and_init();
  
  vector<cmd> validMoves(const Board& currBoard) const;
};


void Player::read(){
  cin >> row >> col >> score >> dig_delay;
  alive = (row >= 0);
}
  
void Player::read_and_init(){
  cin >> row >> col;
  respawn_row = row;
  respawn_col = col;
  alive = true;
  dig_delay = 0;
}

vector<cmd> Player::validMoves(const Board& currBoard) const{
  if(!alive) return vector<cmd>(1,NONE);
  int r,c;
  curr_loc(r,c);
  return currBoard.valid_moves(r,c,dig_delay == 0);
}

#endif
