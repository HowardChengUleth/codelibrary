#include "globals.h"
#include "classes.h"
#include "util.h"

/*******************
 * KG added these, for trimming enemy programs.
 */

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

////////////////////////////////////////////////////////////////////////////
// Position
////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream &os, const Position &p) {
  os << "<" << (int)p.row << ", " << (int)p.col << ">";
  return os;
}

istream& operator>>(istream &is, Position &p) {
  int row, col;
  is >> row >> col;
  p.row = row;
  p.col = col;
  return is;
}

////////////////////////////////////////////////////////////////////////////
// Move
////////////////////////////////////////////////////////////////////////////
const Move Move::UP = Move(-1,  0, 'T');
const Move Move::DOWN  = Move( 1,  0, 'B');
const Move Move::LEFT  = Move( 0, -1, 'L');
const Move Move::RIGHT = Move( 0,  1, 'R');
const Move Move::NONE  = Move( 0,  0, 'N');
//const Move moves[] = {Move::KG_UP, Move::KG_DOWN, Move::KG_LEFT, Move::KG_RIGHT};

const Move& reverse(const Move &m) {
  if (m == Move::UP)
    return Move::DOWN;
  if (m == Move::DOWN)
    return Move::UP;
  if (m == Move::LEFT)
    return Move::RIGHT;
  if (m == Move::RIGHT)
    return Move::LEFT;
  return Move::NONE;
}

const Move& getMove(cmd c) {
  switch (c) {
  case LEFT:  return Move::LEFT;
  case RIGHT : return Move::RIGHT;
  case UP : return Move::UP;
  case DOWN : return Move::DOWN;
  default: return Move::NONE;
  }
}

////////////////////////////////////////////////////////////////////////////
// Board
////////////////////////////////////////////////////////////////////////////
istream& operator>>(istream &is, Board &b) {
  string input;
  for (int row = 0; row < HEIGHT; row++) {
    is >> input;
    for (int col = 0; col < WIDTH; col++) {
      uchar c = 0;
      switch (input[col]) {
      case EMPTY:
	c=1; break;
      case LADDER:
	c=2; break;
      case GOLD:
	c=3; break;
      case BRICK:
	c=154; break;
      case REMOVED_BRICK:
	c=155; break; // this will typically not be used.
      }
      b.cells[row][col] = c;
    }
  }
  return is;
}

ostream& operator<<(ostream &os, const Board &b) {    
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      os << b.visible(row, col);
    }
    os << endl;
  }
  
  return os;
}

bool Board::canStand(int r, int c) const {
  if(!valid_indices(r, c)) return false;

  switch(visible(r,c)){
  case BRICK: return false;
  case LADDER: return true;
  default:;
  }
    
  if(r == HEIGHT-1) return true;
  switch(visible(r+1, c)) {
  case BRICK: return true;
  case LADDER: return true;
  case REMOVED_BRICK:
    for(size_t i=0;i<enemies.size();i++)
      if(enemies[i].pos.row == r+1 && enemies[i].pos.col == c)
	return true;
  default:;
  }
  return false;
}

uchar Board::visible(int r, int c) const {
  if (!valid_indices(r, c)) return EMPTY;
  return visible(Position(r, c));
}


vector<cmd> Board::valid_moves(int row,int col,bool canDig) const {
  vector<cmd> v(1,NONE);
  if(!canStand(row,col)) return vector<cmd>(1,DOWN); // Must be falling!
  if(visible(row, col) == LADDER){
    if(row != 0 && visible(row-1, col) != BRICK)
      v.push_back(UP);
    if(row != HEIGHT-1 && visible(row+1, col) != BRICK)
      v.push_back(DOWN);
  } else {
    if(row != HEIGHT-1 && visible(row+1, col) == LADDER)
      v.push_back(DOWN);
  }
    
  if(col > 0 && visible(row, col-1) != BRICK)
    v.push_back(LEFT);
  if(col < WIDTH-1 && visible(row, col+1) != BRICK)
    v.push_back(RIGHT);
    
  if(!canDig) return v;
    
  if(row == HEIGHT-1) return v; // No can dig
    
  if (visible(row+1, col) != BRICK && visible(row+1, col) != LADDER) return v;
  if(col > 0)
    if(visible(row, col-1) != BRICK && visible(row, col-1) != LADDER)
      if(visible(row+1, col-1) == BRICK)
	v.push_back(DIG_LEFT);
    
  if(col < WIDTH-1)
    if(visible(row, col+1) != BRICK && visible(row, col+1) != LADDER)
      if(visible(row+1, col+1) == BRICK)
	v.push_back(DIG_RIGHT);
    
  return v;
}

vector<Move> Board::valid_movesM(int row, int col, bool canDig) const {
  vector<cmd> vec = valid_moves(row, col, canDig);

  vector<Move> vm;
  for (size_t i = 0; i < vec.size(); i++) {
    vm.push_back(getMove(vec[i]));
  }
  return vm;
}

/*
void Board::BFS_no_fall(){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      for(int k=0;k<HEIGHT;k++)
	for(int l=0;l<WIDTH;l++)
	  dist_no_fall[i][j][k][l] = oo;   // Not reachable

  // Calculate the distance without any falling. This is for "reversible path"
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(canStand(i, j))
	SS_BFS_no_fall(i,j,dist_no_fall[i][j]);

  // This gross mess updates nextmove_dont_use_me_use_Goto_instead[][][][]
  for (int i = 0; i < HEIGHT; i++) 
    for (int j = 0; j < WIDTH; j++)
      if (canStand(i,j)) 
	for (int k = 0; k < HEIGHT; k++) 
	  for (int l = 0; l < WIDTH; l++)
	    if (canStand(k,l)) {
	      nextmove_dont_use_me_use_Goto_instead[i][j][k][l].clear();
	      if (k == i && j == l) {
		nextmove_dont_use_me_use_Goto_instead[i][j][k][l].push_back(NONE); continue;
	      }
	      for (int dir = 0; dir < 4; dir++) {
		int ii = i + di[dir], jj = j + dj[dir];
		if (valid_indices(ii, jj)){
		  if (1 + dist_no_fall[ii][jj][k][l] == dist_no_fall[i][j][k][l])
		    nextmove_dont_use_me_use_Goto_instead[i][j][k][l].push_back(delta[dir]);
		}
	      }
	    }
}

// Never dig a hole. Never fall.
void Board::SS_BFS_no_fall(int r,int c,int dist[HEIGHT][WIDTH]) {
  queue<pii> q;
    
  dist[r][c] = 0;
  q.push(make_pair(r,c));
  
  while(!q.empty()){
    pii t = q.front(); q.pop();
    int I = t.first, J = t.second;
      
    vector<cmd> v = valid_moves(I,J,false);
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(canStand(nI,nJ) && dist[nI][nJ] == oo){
	dist[nI][nJ] = dist[I][J] + 1;
	q.push(make_pair(nI,nJ));
      }
    }
  }
}

// Used by BFS_with_fall
void Board::update_and_add(PQ& pq,vector<cmd> moves[HEIGHT][WIDTH],
			   int dist2[HEIGHT][WIDTH][11],int nDist,
			   int nI,int nJ,int nD,cmd nC){
  if(dist2[nI][nJ][nD] < nDist) return; // We already have better!
    
  if(dist_with_fall[nI][nJ] == nDist) // Add this move to our list!
    if(find(moves[nI][nJ].begin(),moves[nI][nJ].end(),nC) == moves[nI][nJ].end())
      moves[nI][nJ].push_back(nC);
    
  if(dist_with_fall[nI][nJ] > nDist){
    dist_with_fall[nI][nJ] = nDist;
    moves[nI][nJ].clear();       // Remove old results
    moves[nI][nJ].push_back(nC); // Put me in!
  }
    
  if(dist2[nI][nJ][nD] > nDist){
    dist2[nI][nJ][nD] = nDist;
    pq.push(make_tuple(nDist,nI,nJ,nD,nC));
  }
}

// Calculate the distance if we are allowed to fall. If we are "inside" of a
//  brick then we will fall. Note that this means that this brick must have
//  been broken.
// Moreover, this will break bricks and attempt to commit suicide
void Board::SS_BFS_with_fall(int r,int c,int delay){
  PQ pq;
  
  vector<cmd> moves[HEIGHT][WIDTH];
  int dist2[HEIGHT][WIDTH][11];
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      for(int k=0;k<=10;k++) dist2[i][j][k] = oo;
      dist_with_fall[i][j] = oo;
    }
    
  dist2[r][c][delay] = dist_with_fall[r][c] = 0;
  moves[r][c].push_back(NONE);
  pq.push(make_tuple(0,r,c,delay,NONE));
    
  pair<cmd,int> suicide = suicide_move();
  suicide.second += PLAYER_RESPAWN_TIME + 5;
  
  update_and_add(pq,moves,dist2,suicide.second,
		 respawn_row,respawn_col,0,suicide.first);
  
  int I,J,D,myDist;
  cmd C;
  for(bool first=true;!pq.empty();first=false){
    tie(myDist,I,J,D,C) = pq.top(); pq.pop();
      
    if(myDist > dist2[I][J][D] && !first) continue;
      
    vector<cmd> v = valid_moves(I,J,(D == 0));
      
    // Deal with one very special case:
    //  If there is a hole here, but it may not "actually" be here...
    //  This implementation only works for when you will be WALKING
    //   over the hole, not when you jump from up high into it...
      
    for(int CMD=LEFT;CMD<=RIGHT;CMD++)
      if(find(v.begin(),v.end(),CMD) != v.end())
	if(visible(I+1,J+dj[CMD]) == REMOVED_BRICK){
	  cmd nC = (first ? NONE : C);
	  int nI = I;
	  int nJ = J+2*dj[CMD];
	  if(!valid_indices(I,J+2*dj[CMD])) continue;
	  int respawn = brick_respawns_in(I+1, J+dj[CMD]);
	  if(respawn < 0){
	    cerr << respawn << endl;
	      
	  }
#ifndef FINAL_VERSION
	  assert(respawn >= 0);
#endif
	  int nD = max(0,D - respawn);
	  int nDist = respawn + 2; // NONEs + 2 CMDs
	  update_and_add(pq,moves,dist2,nDist,nI,nJ,nD,nC);
	}
      
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(!valid_indices(nI,nJ)) continue;
	
      // The next line is 8 because:
      //  first you dig (at turn t), then move left (at turn t+1), then down (at turn t+2)
      int nD = max(0,(v[k] == DIG_LEFT || v[k] == DIG_RIGHT ? 8 : D-1));
      int nDist = myDist + dijkstra_dist[v[k]];
      cmd nC = (first ? v[k] : C);
	
      update_and_add(pq,moves,dist2,nDist,nI,nJ,nD,nC);
    }
  }
    
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      sort(moves[i][j].begin(),moves[i][j].end());
      nextmove_dont_use_me_use_Goto_instead[r][c][i][j] = moves[i][j];
    }
    
}
 // End of commented out section of old distance functions
*/

/*  
void Board::initSCC()
{
  bool reachable[HEIGHT][WIDTH][HEIGHT][WIDTH];
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      for (int k = 0; k < HEIGHT; k++) {
	for (int l = 0; l < WIDTH; l++) {
	  reachable[i][j][k][l] = false;
	}
      }
    }
  }
  for (int r = 0; r < HEIGHT; r++) {
    for (int c = 0; c < WIDTH; c++) {
      SS_BFS_with_fall(r, c, 0);
      for (int k = 0; k < HEIGHT; k++) {
	for (int l = 0; l < WIDTH; l++) {
	  cerr << "(" << r << ", " << c << ") -> ("
	       << k << ", " << l << ") = " << dist_with_fall[k][l] << ": ";
	  reachable[r][c][k][l] |= dist_with_fall[k][l] < oo;
	  cerr << (reachable[r][c][k][l] ? "yes" : "no") << endl;
	}
      }
    }
  }
    
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      for (int k = 0; k < HEIGHT; k++) {
	for (int l = 0; l < WIDTH; l++) {
	  if (reachable[i][j][k][l] && reachable[k][l][i][j]) {
	    uf.merge(uf_index(i,j), uf_index(k,l));
	    cerr << "  Merging (" << i << ", " << j << ")  and ("
		 << k << ", " << l << ")" << endl;
	  }
	}
      }
    }
  }

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      cerr << "(" << i << ", " << j << ") = " << scc_index(i, j) << endl;
    }
  }
}
*/

 /*void Board::init(int r,int c,int delay) {
  if(r == -1 && c == -1) return; // Doesn't matter, I'm dead...
  BFS_no_fall();
  SS_BFS_with_fall(r,c,delay);
  }*/

/////////////////////////////////////////////////////////////////////////
// Player
/////////////////////////////////////////////////////////////////////////
istream& operator>>(istream &is, Player &p) {
  int mcount;
  is >> p.pos >> p.score >> mcount;
  p.mason_count = mcount;
  return is;
}

ostream& operator<<(ostream &os, const Player &p) {
  os << "Player(" << p.pos << ", " << p.score << ", " << (int)p.death_count << ", " << (int)p.mason_count << ")";
  return os;
}

/////////////////////////////////////////////////////////////////////////
// Enemy
/////////////////////////////////////////////////////////////////////////
istream& operator>>(istream &is, Enemy &p) {
  int m;
  is >> p.pos >> m;  p.master = m;
  return is;
}

ostream& operator<<(ostream &os, const Enemy &p) {
  os << "Enemy(" << p.pos << ", " << (int)p.ip << ", " << (int)p.death_count << ", " << (int)p.master << ", " << p.stack << ")";
  return os;
}

/////////////////////////////////////////////////////////////////////////
// Game
/////////////////////////////////////////////////////////////////////////
istream& operator>>(istream &is, Game &g) {
  is >> g.turns >> g.board >> g.me >> g.you;
  int enemyCount;
  is >> enemyCount;
  g.enemyPos.clear();
  g.enemyProgram.clear();
  
  Position enemyPos;
  string enemyProgram;
  
  for (int i = 0; i < enemyCount; i++) {
    is >> enemyPos; // >> enemyProgram;
    // Accounting for empty program
    getline(cin, enemyProgram);
    trim(enemyProgram);
    if (enemyProgram.length() < 1)
    	enemyProgram = "N";
    g.enemyPos.push_back(enemyPos);
    g.enemyProgram.push_back(enemyProgram);
  }
  return is;
}

ostream& operator<<(ostream &os, const Game &g) {
  os << "=============================================" << endl;
  os << g.board;
  os << "Me: " << g.me << endl;
  os << "You: " << g.you << endl;
  for (size_t i = 0; i < g.enemyPos.size(); i++) {
    os << "Enemy: " << g.enemyPos[i] << " " << g.enemyProgram[i] << endl;
  }
  return os;
}

/////////////////////////////////////////////////////////////////////////
// State
/////////////////////////////////////////////////////////////////////////
State::State(const Game &g) : turn(0), board(g.board), me(g.me), you(g.you) {
  for (unsigned int i = 0; i < g.enemyPos.size(); i++) {
    enemies.push_back( Enemy(g.enemyPos[i]) );
  }
  for (int row = 0; row < HEIGHT; row++)
    for (int col = 0; col < WIDTH; col++) {
      visited[row][col] = false;
    }
  
  
  /*
    cerr << "Me: " << me << " " << me.isDead() << endl;
    
    if (!me.isDead())
    if (!visited[me.pos.row][me.pos.col]) {
    me.score++;
    }
    
    if (!you.isDead())
    if (!visited[you.pos.row][you.pos.col]) {
    you.score++;
    }
    
    if (!me.isDead())
    visited[me.pos.row][me.pos.col] = true;
    
    if (!you.isDead())
    visited[you.pos.row][you.pos.col] = true;
    
    cerr << "Me: " << me << " " << me.score << endl;
  */
}

bool State::fallingDude(const Dude &p) const {
  // Runners and enemies are also considered to be standing on something if they are on the bottom-most row.
  if (p.pos.row == HEIGHT-1) {
    return false;
  }
  
  //They will fall down, one cell each turn if they are not either on a ladder ...  A runner or enemy is considered to be on a ladder if it's in a cell that contains Ladder.
  if (board.visible(p.pos) == LADDER) {
    return false;
  }
  
  //or standing on something.  A runner or enemy is considered to be standing on something if it's above a cell containing either Brick or Ladder
  Position below = Move::DOWN.apply(p.pos);
  if ((board.visible(below) == BRICK) || (board.visible(below) == LADDER)) {
    return false;
  }
  
  
  // or Removed_Brick with an enemy trapped in it.
  if (board.visible(below) == REMOVED_BRICK) {
    for (size_t i = 0; i < enemies.size(); i++)
      if (below == enemies[i].pos) {
	return false;
      }
  }
  
  return true;
}

bool State::fallingEnemy(const Enemy &p) const {
  if ( board.visible(p.pos) == REMOVED_BRICK ) { // enemy is trapped
    return false;
  } else {
    return fallingDude(p);
  }
}

bool State::legal(const Position &p) const
{
  return valid_indices(p.row, p.col);
}

bool State::canNonFall(const Dude &p, const Move &move) const {

  //cerr << "Applying Move: " << move.label << endl;
  
  if (move == Move::NONE)
    return true;
  
  Position newPos = move.apply(p.pos);
  //cerr << "New Position " << newPos << endl;
  
  //2.  A runner can't move off the map. For example, a runner can't move left from the column zero or climb up from row 0, even if there's a ladder to climb on.
  if (!legal(newPos)) {
    //cerr << "Illegal Position " << newPos << endl;
    return false;
  }
  
  // 3. A runner can?t move into a cell that contains Brick and can't move down into a cell containing Removed_Brick if it already has an enemy trapped in it.
  // KG Note:  If the cell contained remove brick with no enemy, this case would be handled by falling.
  if (board.visible(newPos) == BRICK) {
    //cerr << "New Position is bricked" << endl;
    return false;
  }
  
  // 4. If a runner is in a cell containing Ladder, the runner can move in any of the four directions (Left, Right, Top or Bottom), provided the previous conditions are not violated.
  if (board.visible(p.pos) == LADDER) {
    //cerr << "New Position is ladder, applied" << endl;
    //newp.pos = newPos;
    return true;
  }
  
  // 5. If a runner is above a cell containing Ladder, the runner can perform the Left, Right and Bottom commands, provided the previous conditions are not violated.
  // 6. If a runner is above a cell containing Brick, or Removed_Brick with an enemy trapped in the cell, the runner can perform the Left and Right commands, provided the previous conditions are not violated.
  
  // KG Note:  Trying to summarize rules here.  Given that all checks above have passed, I can't see what would possibly restrict
  // the runner from moving left or right.  And so we need only check for a move down, that a ladder block is below the runner.
  
  if (move == Move::DOWN) {
    if ( board.visible(newPos) == LADDER ) {
      //newp.pos = newPos;
      return true;
    } else {
      return false;
    }
  }
  
  if ( (move == Move::LEFT) || (move == Move::RIGHT) ) {
    //cerr << "New Position is left or right, applied" << endl;
    //newp.pos = newPos;
    return true;
  }
  
  return false;
}

void State::applyNonFall(const Dude &p, Dude &newp, const Move &move) const {
  if (canNonFall(p, move)) {
    Position newPos = move.apply(p.pos);
    newp.pos = newPos;
  }
}

void State::applyMoveDude(const Dude &p, Dude &newp, const cmd &command) const {
  if (p.isDead()) {
#if DEBUG
    cerr << "Player is dead!" << endl;
#endif
    return;
  }

  // If a character is falling, it moves down one cell.
  if ( fallingDude(p) ) {
#if DEBUG
    cerr << "Player is falling!" << endl;
#endif
    newp.pos = Move::DOWN.apply(p.pos);
  } else {
#if DEBUG
    cerr << "Player is not falling!" << endl;
#endif
    const Move &move = getMove(command);
    applyNonFall(p, newp, move);
  }
}

void State::update(const Move *prevMove[HEIGHT][WIDTH],
	    Position prevPosition[HEIGHT][WIDTH],
	    queue< pair<Position, int> > &myqueue, const Position &position,
	    int step, const Position &prev, const Move &move) const {
  if (prevMove[(uchar)position.row][(uchar)position.col] == &Move::NONE) {
    prevPosition[(uchar)position.row][(uchar)position.col] = prev;
    prevMove[(uchar)position.row][(uchar)position.col] = &move;
    myqueue.push( pair<Position, int>(position, step) );
  }
}

bool State::fallingPursuit(const Position &pos) const {
  
  // Runners and enemies are also considered to be standing on something if they are on the bottom-most row.
  if (pos.row == HEIGHT-1)
    return false;
  
  //They will fall down, one cell each turn if they are not either on a ladder ...  A runner or enemy is considered to be on a ladder if it's in a cell that contains Ladder.
  if (board.visible(pos) == LADDER)
    return false;
  
  //or standing on something.  A runner or enemy is considered to be standing on something if it's above a cell containing either Brick or Ladder
  Position below = Move::DOWN.apply(pos);
  if ((board.visible(below) == REMOVED_BRICK) || (board.visible(below) == BRICK) || (board.visible(below) == LADDER))
    return false;
  
  return true;
}

pair<const Move *, int> myGetPursuitMove(const Enemy &enemy,
					 const Player &player,
					 const Move **moves, int limit)  {
  // Is our runner died?
  if (player.isDead()) {
    return pair<const Move *, int>(&Move::NONE, 0);
  }
  
  int eRow = enemy.pos.row,  eCol = enemy.pos.col;
  int pRow = player.pos.row, pCol = player.pos.col;

  assert(!enemy.isDead());
  
  int distance = dist_orig_board_reversible[eRow][eCol][pRow][pCol];
  if(distance > limit) return pair<const Move *, int>(&Move::NONE, 0);
  assert(distance != 0);
  
  for (int m = 0; m < 4; m++) {
    int Di = moves[m]->dRow, Dj = moves[m]->dCol;
    int nI = eRow + Di, nJ = eCol + Dj;
    if(!valid_indices(nI,nJ)) continue;
    if(dist_orig_board_reversible[eRow][eCol][pRow][pCol] - 1 ==
       dist_orig_board_reversible[nI][nJ][pRow][pCol]){
      vector<Move> v = g.board.valid_movesM(nI,nJ,false);
      if(find(v.begin(),v.end(),reverse(*moves[m])) == v.end()) continue;
      v = g.board.valid_movesM(eRow,eCol,false);
      if(find(v.begin(),v.end(),*moves[m]) == v.end()) continue;
      return make_pair(moves[m],distance);
    }
  }
  
  assert(false);
  return make_pair(moves[0],-1);
}

pair<const Move *, int> State::getPursuitMove(const Enemy &enemy,
					      const Player &player,
					      const Move **moves, int limit)  const {
  return myGetPursuitMove(enemy,player,moves,limit);
  const Move *prevMove[HEIGHT][WIDTH];
  Position prevPosition[HEIGHT][WIDTH];
  queue< pair<Position, int> > myqueue;
  
  // Is our runner died?
  if (player.isDead()) {
    if(myGetPursuitMove(enemy,player,moves,limit) != pair<const Move *, int>(&Move::NONE, 0)){
      cerr << "NOOOOOOOOOOO" << endl;
      cerr << myGetPursuitMove(enemy,player,moves,limit).second << " " << 0 << endl;
    }
    return pair<const Move *, int>(&Move::NONE, 0);
  }
  
  for (int row = 0; row < HEIGHT; row++)
    for (int col = 0; col < WIDTH; col++) {
      prevPosition[row][col] = NO_POS;
      prevMove[row][col] = &Move::NONE;
    }
  
  myqueue = queue< pair<Position, int> >();
  Position target(NO_POS);
  
  update(prevMove, prevPosition, myqueue, enemy.pos, 0, NO_POS, Move::NONE);
  
  while (!myqueue.empty()) {
    pair<Position, int> positionInt = myqueue.front(); myqueue.pop();
    Position position = positionInt.first;
    int step = positionInt.second;
    if (player.pos == position) {
      target = position;
      break;
    }
    
    // Iterate through possible moves in order (LEFT, RIGHT, TOP, BOTTOM)
    
    if (step < limit) {
      for (int m = 0; m < 4; m++) {
	Position newPosition = (*moves[m]).apply(position);
	if (!legal(newPosition))
	  continue;
	if (fallingPursuit(newPosition))
	  continue;
	if ((board.visible(newPosition) == BRICK) || (board.visible(newPosition) == REMOVED_BRICK))
	  continue;
	if ((moves[m] == &Move::UP) && (board.visible(position) != LADDER))
	  continue;
	if ((moves[m] == &Move::DOWN) && (board.visible(position) == LADDER) && ((board.visible(newPosition) == EMPTY) || (board.visible(newPosition) == GOLD)))
	  continue;
	update(prevMove, prevPosition, myqueue, newPosition, step+1, position, *moves[m]);
      }
    }
  }
  
  if (target == NO_POS) {
    if(myGetPursuitMove(enemy,player,moves,limit) != pair<const Move *, int>(&Move::NONE, 0)){
      cerr << "NOOOOOOOOOOO  1" << endl;
      cerr << myGetPursuitMove(enemy,player,moves,limit).second << " " << 0 << " " << limit << endl;
      cerr << board << endl;
      cerr << int(enemy.pos.row) << " " << int(enemy.pos.col) << endl;
      cerr << int(player.pos.row) << " " << int(player.pos.col) << endl;
      cerr << board << endl;
      cerr << "Player = " << player << endl;
      cerr << "Enemy = " << enemy << endl;
      exit(1);
    }
    return pair<const Move *, int>(&Move::NONE, 0);
  }
  
  
  // Returning back through previous positions from the closest gold
  Position current = target, previous = target;
  int count = 0;
  while (!(current == enemy.pos)) {
    count++;
    previous = current;
    current = prevPosition[(uchar)current.row][(uchar)current.col];
  }
  
  // Make a move to the closest gold
  if(myGetPursuitMove(enemy,player,moves,limit) != pair<const Move *, int>(prevMove[(uchar)previous.row][(uchar)previous.col], count)){
    cerr << "NOOOOOOOOOOO 2" << endl;
    cerr << board << endl;
    cerr << myGetPursuitMove(enemy,player,moves,limit).second << " " << count << endl;
    cerr << "Player = " << player << endl;
    cerr << "Enemy = " << enemy << endl;
    cerr << (myGetPursuitMove(enemy,player,moves,limit).first)->label << " " <<
      (prevMove[(uchar)previous.row][(uchar)previous.col])->label << endl;
    exit(1);
  }
  return pair<const Move *, int>(prevMove[(uchar)previous.row][(uchar)previous.col], count);
}

void State::applyMoveEnemy(const Enemy &enemy, Enemy &newEnemy,
			   string program, bool left, int turn) const {
  if (enemy.isDead()) {
#if DEBUG
    cerr << "Enemy " << enemy << " is dead!" << endl;
#endif
    return;
  }
  // check if stuck
  if (board.visible(enemy.pos) == REMOVED_BRICK) {
    return;
  }
  // If a character is falling, it moves down one cell.
  if ( fallingEnemy(enemy) ) {
#if DEBUG
    cerr << "Enemy " << enemy << " is falling!" << endl;
#endif

    newEnemy.pos = Move::DOWN.apply(enemy.pos);

    // accounting for empty enemy stack.  I think this one is ok though.

    newEnemy.ip++;
    if (newEnemy.ip >= program.size())
      newEnemy.ip = 0;


    newEnemy.lastMove = turn;
  } else {
    if ( turn - enemy.lastMove < 2 ) {
#if DEBUG
      cerr << "Enemy " << enemy << " is waiting her turn!" << turn << endl;
#endif
      return;
    }
    
    
    const Move *m = NULL;
    
    const Move **moves;
    const Move *leftMoves[] = {&Move::UP, &Move::RIGHT, &Move::DOWN, &Move::LEFT};
    const Move *rightMoves[] = {&Move::UP, &Move::LEFT, &Move::DOWN, &Move::RIGHT};
    
    if (left) {
      moves = leftMoves;
    } else {
      moves = rightMoves;
    }
    
    pair<const Move *, int> getMe = getPursuitMove(enemy, me, moves, (enemy.master == 0) ? 4 : (enemy.master == 1) ? 8 : 5);
    pair<const Move *, int> getYou = getPursuitMove(enemy, you, moves, (enemy.master == 0) ? 8 : (enemy.master == 1) ? 4 : 5);
    
#if DEBUG
    cerr << "Get Me: " << getMe.first->label << ", " << getMe.second << endl;
    cerr << "Get You: " << getYou.first->label << ", " << getYou.second << endl;
    cerr << "Master: " << (int)enemy.master << endl;
    cerr << "Equal: " << (getMe.first == getYou.first) << endl;
#endif
    
    if (enemy.master == 0) {
      if (getMe.second > 4)
	getMe.first = &Move::NONE;
      if (getYou.second > 8)
	getYou.first = &Move::NONE;
    } else if (enemy.master == 1) {
      if (getMe.second > 8)
	getMe.first = &Move::NONE;
      if (getYou.second > 4)
	getYou.first = &Move::NONE;
    } else {
      if (getMe.second > 5)
	getMe.first = &Move::NONE;
      if (getYou.second > 5)
	getYou.first = &Move::NONE;
    }
    
    
    if (getMe.first == getYou.first) {
      m = getMe.first;
    } else if (getMe.first == &Move::NONE) {
      m = getYou.first;
    } else if (getYou.first == &Move::NONE) {
      m = getMe.first;
    } else if (getMe.second < getYou.second) {
      m = getMe.first;
    } else if (getMe.second > getYou.second) {
      m = getYou.first;
    } else {  // both players are in range, and equidistant
      for (int moveIndex = 0; moveIndex != 4; moveIndex++) {
	if ( (getMe.first == moves[moveIndex]) ||  (getYou.first == moves[moveIndex]) ) {
	  m = moves[moveIndex];
	  break;
	}
      }
    }
    
#if DEBUG
    cerr << "Pursuit Move: " << m->label << endl;
#endif
    
    if (m != &Move::NONE) { // if in pursuit mode, move there
      newEnemy.stack += reverse(*m).label;
    } else {
      char com;
      if (newEnemy.stack.length() > 0) {// otherwise, if stack is not empty, do that move
	    com = newEnemy.stack[ newEnemy.stack.length() - 1];
	    newEnemy.stack = newEnemy.stack.substr(0, newEnemy.stack.length() - 1);
      } else {

    	  // need to account for empty program
    	 //com = 'N';
    	 //if ( program.size() > 0 ) {
    		 com = program[newEnemy.ip++];
	         if (newEnemy.ip >= program.size())
	           newEnemy.ip = 0;
      }
    	 //}
      
      switch (com) {
      case 'L' : m = &Move::LEFT; break;
      case 'R' : m = &Move::RIGHT; break;
      case 'T' : m = &Move::UP; break;
      case 'B' : m = &Move::DOWN; break;
      case 'N' : m = &Move::NONE; break;
      }
    }
    
    applyNonFall(enemy, newEnemy, *m);
    newEnemy.lastMove = turn;
    
  }
}

bool State::canDestroyBrick(const Player &p, const Position &sideCell, const Position &targetCell) const {
  // 1. The runner must be alive.
  
  if (p.isDead())
    return false;
  
  // 2. The runner hasn't destroyed a brick too recently. After successfully destroying a brick, the runner can't destroy another one until 10 turns later. If, for example, a runner destroys a Brick at turn number t, the runner won't be able to destroy another Brick until turn t+10.
  
  if (p.mason_count > 0)
    return false;
  
  // 3. The runner must not be on the bottom-most row. The runner can occupy this row, but there are no bricks below it to destroy.
  if (!legal(targetCell))
    return false;
  
  // 4. The runner must be on a ladder (Ladder in the Runner Cell of the above figures) or standing on something solid (either Brick or Ladder in the Lower Cell of the figure).
  if (board.visible(p.pos) != LADDER)  {
    Position below = Move::DOWN.apply(p.pos);
    if ((board.visible(below) != BRICK) && (board.visible(below) != LADDER)) {
      return false;
    }
  }
  
  // 5. The Side Cell of the figure must not contain Brick or Ladder.
  if ( (board.visible(sideCell) == BRICK) || (board.visible(sideCell) == LADDER) ) {
    return false;
  }
  
  
  // 6. Of course, the Target Cell must contain Brick.
  if (board.visible(targetCell) != BRICK) {
    return false;
  }
  
  //If all these conditions are met, the runner is able to destroy a brick
  return true;
}

State& State::getNewState(const Game &g, cmd myCommand, cmd yourCommand,
			  State &newState) const {
  // this is where the inference happens.
  
  //State newState(*this);
  newState = *this;
  newState.turn = turn+1;
  
  if (!newState.me.isDead())
    if (!visited[(uchar)newState.me.pos.row][(uchar)newState.me.pos.col]) {
      newState.me.score++;
    }
  
  if (!newState.you.isDead())
    if (!visited[(uchar)newState.you.pos.row][(uchar)newState.you.pos.col]) {
      newState.you.score++;
    }
  
  if (!newState.me.isDead())
    newState.visited[(uchar)newState.me.pos.row][(uchar)newState.me.pos.col] = true;
  
  if (!newState.you.isDead())
    newState.visited[(uchar)newState.you.pos.row][(uchar)newState.you.pos.col] = true;

  //1.  Runner and enemy moves are applied.  If a character is falling, it moves down one cell.
  
#if DEBUG
  cerr << "Applying my move" << endl;
#endif
  
  applyMoveDude(me, newState.me, myCommand);
  
#if DEBUG
  cerr << "Applying your move" << endl;
#endif
  
  applyMoveDude(you, newState.you, yourCommand);
  
  for (size_t i = 0; i < enemies.size(); i++) {
    applyMoveEnemy(enemies[i], newState.enemies[i], g.enemyProgram[i], g.enemyPos[i].col < 12, newState.turn);
  }
  
  
  //2.  Runners are killed if they passed through an enemy while moving. This can happen if the runner and enemy exchange positions during the turn.
  
  if (!newState.me.isDead())
    for (size_t i = 0; i < enemies.size(); i++)
      if ((me.pos == newState.enemies[i].pos) && (newState.me.pos == enemies[i].pos)) {
	killDude(newState.me, 51);
	break;
      }
  
  
  if (!newState.you.isDead())
    for (size_t i = 0; i < enemies.size(); i++)
      if ((you.pos == newState.enemies[i].pos) && (newState.you.pos == enemies[i].pos)) {
	killDude(newState.you, 51);
	break;
      }
  
  
  // Add score for new cell, if necessary.
  /*
    if (!newState.me.isDead())
    if (!newState.visited[newState.me.pos.row][newState.me.pos.col]) {
    newState.me.score++;
    }
    
    if (!newState.you.isDead())
    if (!newState.visited[newState.you.pos.row][newState.you.pos.col]) {
    newState.you.score++;
    }
    
    if (!newState.me.isDead())
    newState.visited[newState.me.pos.row][newState.me.pos.col] = true;
    
    if (!newState.you.isDead())
    newState.visited[newState.you.pos.row][newState.you.pos.col] = true;
  */
  
  //3.  If a runner successfully performs Dig_Left or Dig_Right, the Brick is replaced by Removed_Brick.
  
  if (myCommand == DIG_LEFT) {
    Position sideCell = Move::LEFT.apply(me.pos);
    Position targetCell = Move::DOWN.apply(sideCell);
    if ( canDestroyBrick(me, sideCell, targetCell) ) {
      newState.board.removeBrick(targetCell, true);
      newState.me.mason_count = 10;
    }
  } else if (myCommand == DIG_RIGHT) {
    Position sideCell = Move::RIGHT.apply(me.pos);
    Position targetCell = Move::DOWN.apply(sideCell);
    if ( canDestroyBrick(me, sideCell, targetCell) ) {
      newState.board.removeBrick(targetCell, true);
      newState.me.mason_count = 10;
    }
  }
  
  
  if (yourCommand == DIG_LEFT) {
    Position sideCell = Move::LEFT.apply(you.pos);
    Position targetCell = Move::DOWN.apply(sideCell);
    if ( canDestroyBrick(you, sideCell, targetCell) ) {
      newState.board.removeBrick(targetCell, false);
      newState.you.mason_count = 10;
    }
  } else if (yourCommand == DIG_RIGHT) {
    Position sideCell = Move::RIGHT.apply(you.pos);
    Position targetCell = Move::DOWN.apply(sideCell);
    if ( canDestroyBrick(you, sideCell, targetCell) ) {
      newState.board.removeBrick(targetCell, false);
      newState.you.mason_count = 10;
    }
  }
  
  // Brick and Gold values are restored and runners and enemies are respawned if the appropriate number of turns have passed since they were destroyed, collected or killed.
  // Respawning runners and enemies first.
  
  //cerr << "Respawn me, bitch!!" << endl;
  respawn(newState.me, g.me);
  //cerr << "Respawn you, bitch!!" << endl;
  respawn(newState.you, g.you);
  
  for (size_t i = 0; i < enemies.size(); i++)
    respawn( newState.enemies[i], g.enemyPos[i] );
  
  //cerr << newState.board << endl;
  //cerr << "SANITY CHECK" << endl;
  
  //cerr << (int)board.cells[4][6] << endl;
  
  // Now bricks and gold
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      Position pos(row, col);
      // Gold
      if (board.removedGold(pos)) {
	/*
	  cerr << "Adding back gold at " << pos << endl;
	  cerr << board.visible(pos) << endl;
	  cerr << (int)board[pos] << endl;
	  cerr << (int)newState.board[pos] << endl;
	  cerr << (int)board.cells[pos.row][pos.col] << endl;
	*/
	newState.board[pos]--;
	//cerr << (int)board[pos] << endl;
	//cerr << (int)newState.board[pos] << endl;
      }
      // Brick
      if (board.visible(pos) == REMOVED_BRICK) {
	newState.board[pos]--;
	if ((newState.board[pos] == 204) || (newState.board[pos] == 179))
	  newState.board[pos] = 154;
	
	if (newState.board.visible(pos) == BRICK) { // a brick has been restored, kill anyone inside
	  if ( newState.me.pos == pos ) {  // kill me
	    killDude(newState.me, 50);
	    if ((board[pos] == 205) || (board[pos] == 180))
	      newState.you.score += 300;
	  }
	  if ( newState.you.pos == pos ) {  // kill you
	    killDude(newState.you, 50);
	    if ((board[pos] == 205) || (board[pos] == 155))
	      newState.me.score += 300;
	  }
	  for (size_t i = 0; i < enemies.size(); i++)
	    if ( newState.enemies[i].pos == pos ) { // kill him
	      killDude(newState.enemies[i], 25);
	      newState.enemies[i].stack = "";
	      newState.enemies[i].ip = 0;
	      if (board[pos] == 205) {            // both are awarded points
		newState.me.score += 20;
		newState.you.score += 20;
		newState.enemies[i].master = -1;
	      } else if (board[pos] == 180) {   // you are awarded points
		newState.you.score += 20;
		newState.enemies[i].master = 1;
	      } else if (board[pos] == 155) {   // I am awarded points
		newState.me.score += 20;
		newState.enemies[i].master = 0;
	      }
	    }
	}
      }
    }
  }
  
  //cerr << newState.board << endl;
  
  // 5.  Runners collect gold if they have moved into a cell containing gold.
  
  if (!newState.me.isDead())
    if ( newState.board.visible(newState.me.pos) == GOLD ) {
      newState.me.score += 100;
    }

  if (!newState.you.isDead())
    if ( newState.board.visible(newState.you.pos) == GOLD ) {
      newState.you.score += 100;
    }

  if (!newState.me.isDead())
    if ( newState.board.visible(newState.me.pos) == GOLD ) {
      newState.board[newState.me.pos] = 153;  // remove gold for 150 turns
    }

  if (!newState.you.isDead())
    if ( newState.board.visible(newState.you.pos) == GOLD ) {
      newState.board[newState.you.pos] = 153;  // remove gold for 150 turns
    }
  
  
  
  if (!newState.me.isDead())
    if (!newState.visited[(uchar)newState.me.pos.row][(uchar)newState.me.pos.col]) {
      newState.me.score++;
    }
  
  if (!newState.you.isDead())
    if (!newState.visited[(uchar)newState.you.pos.row][(uchar)newState.you.pos.col]) {
      newState.you.score++;
    }
  
  if (!newState.me.isDead())
    newState.visited[(uchar)newState.me.pos.row][(uchar)newState.me.pos.col] = true;
  
  if (!newState.you.isDead())
    newState.visited[(uchar)newState.you.pos.row][(uchar)newState.you.pos.col] = true;
  
  
  
  // 6.  A runners dies if it now occupies the same cell as an enemy.
  
  newState.checkForCollision(newState.me);
  newState.checkForCollision(newState.you);
  
  if (newState.me.mason_count > 0)
    newState.me.mason_count--;
  
  if (newState.you.mason_count > 0)
    newState.you.mason_count--;
  
  /*
    if (!newState.me.isDead())
    if (!newState.visited[newState.me.pos.row][newState.me.pos.col]) {
    newState.me.score++;
    }
    
    if (!newState.you.isDead())
    if (!newState.visited[newState.you.pos.row][newState.you.pos.col]) {
    newState.you.score++;
    }
    
    if (!newState.me.isDead())
    newState.visited[newState.me.pos.row][newState.me.pos.col] = true;
    
    if (!newState.you.isDead())
    newState.visited[newState.you.pos.row][newState.you.pos.col] = true;
  */
  return newState;
}

istream& readInState(istream &is, State &g, int enemyCount) {
  is >> g.turn >> g.board >> g.me >> g.you;
  g.enemies.clear();
  
  for (int i = 0; i < enemyCount; i++) {
    Enemy e;
    is >> e;
    g.enemies.push_back(e);
  }
  return is;
}

ostream& operator<<(ostream &os, const State &s) {
  char newboard[HEIGHT][WIDTH];
  os << "Turn: " << s.turn << endl;
  os << "=============================================" << endl;
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      newboard[row][col] = s.board.visible(row, col);
    }
  }
  if (!s.me.isDead())
    newboard[(uchar)s.me.pos.row][(uchar)s.me.pos.col] = 'M';
  if (!s.you.isDead())
    newboard[(uchar)s.you.pos.row][(uchar)s.you.pos.col] = 'Y';
  for (size_t i = 0; i < s.enemies.size(); i++) {
    if (!s.enemies[i].isDead())
      newboard[(uchar)s.enemies[i].pos.row][(uchar)s.enemies[i].pos.col] = 'E';
  }
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      os << newboard[row][col];
    }
    os << endl;
  }
  os << "=============================================" << endl;
  os << "Me: " << s.me << endl;
  os << "You: " << s.you << endl;
  for (size_t i = 0; i < s.enemies.size(); i++) {
    os << "Enemy: " << s.enemies[i] << endl;
  }
  
  return os;
}


/////////////////////////////////////////////////////////////////////////
// StateEngine
/////////////////////////////////////////////////////////////////////////
#include <fstream>

void StateEngine::next(istream &is, const string &myMove) {
  
  //State *prev = gameStatus;
  
  //gameStatus = new State();
  
  State tempStatus;
  
  readInState(cin, *gameStatus, g.enemyPos.size());
  
#if DEBUG
  cerr << "Game Status" << endl;
  cerr << *gameStatus << endl;
#endif
  
  if (ourStatus) {
    
    cmd myCommand = getCommand(myMove);
    cmd yourCommand;
    
    bool ok = false;

    for (int c = UP; c <= NONE; c++) {
      yourCommand = static_cast<cmd>(c);
      //tempState = prev->getNewState(g, myCommand, yourCommand);
      ourStatus->getNewState(g, myCommand, yourCommand, tempStatus);
      
#if DEBUG
      cerr << "OurStatus:  Move " << names[myCommand] << " " << names[yourCommand] << endl;
      cerr << tempStatus << endl;
#endif
      
      if ( equivalent(*gameStatus, tempStatus) ) {
	*ourStatus = tempStatus;
	ourStatus->me.score = gameStatus->me.score;
	ourStatus->you.score = gameStatus->you.score;
	ok = true;
	break;
      }
    }
    
    if (!ok) {
      cerr << "No Match!" << endl;
      cerr << "Game Status" << endl;
      cerr << *gameStatus << endl;

      ofstream out("error.txt");
      out << "No Match!" << endl;
      out << "Game Status" << endl;
      out << *gameStatus << endl;
      out << "=========================================" << endl;
      
      for (int c = UP; c <= NONE; c++) {
	yourCommand = static_cast<cmd>(c);
	//tempState = prev->getNewState(g, myCommand, yourCommand);
	ourStatus->getNewState(g, myCommand, yourCommand, tempStatus);
	
//#if DEBUG
	out << "OurStatus:  Move " << names[myCommand] << " " << names[yourCommand] << endl;
	out << tempStatus << endl;
	out << "==============================================" << endl;
//#endif
	
	if ( equivalent(*gameStatus, tempStatus, true) ) {
	  *ourStatus = tempStatus;
	  ok = true;
	  break;
	}
      }

#ifndef FINAL_VERSION
      assert(false);      
#endif
      if (killGameOnError) {
#ifndef FINAL_VERSION
	exit(1);
#endif
      } else
	*ourStatus = *gameStatus;
    } else {
#if DEBUG
      cerr << "Inferred Opponent move: " << names[yourCommand] << endl;
#endif
    }
    
  } else {
    ourStatus =  new State(*gameStatus);
  }
}

/////////////////////////////////////////////////////////////////////////
// Lookahead
/////////////////////////////////////////////////////////////////////////
void Lookahead::getViableCommands( const State &status, const Player &p,
				   vector<cmd> &commands ) {
  commands.clear();
  commands.push_back(NONE); // can always do nothing
  
  if (p.isDead()) // dead people can't play
    return;
  
  if (status.fallingDude(p)) // falling people can't play
    return;
  
  cmd moveCommands[] = {UP, LEFT, RIGHT, DOWN};
  
  for (int i = 0; i != 4; i++) {
    if (status.canNonFall(p, getMove(moveCommands[i])))
      commands.push_back(moveCommands[i]);
  }
  
  Position sideCell = Move::LEFT.apply(p.pos);
  Position targetCell = Move::DOWN.apply(sideCell);
  if ( status.canDestroyBrick(p, sideCell, targetCell) ) {
    commands.push_back(DIG_LEFT);
  }
  
  sideCell = Move::RIGHT.apply(p.pos);
  targetCell = Move::DOWN.apply(sideCell);
  
  if ( status.canDestroyBrick(p, sideCell, targetCell) ) {
    commands.push_back(DIG_RIGHT);
  }
}

double Lookahead::boundedLookaheadScoreNewDecay( const Game &g, const State &status,
					 int depth, int max_depth,
					 int turns_remaining, int oldScore, double currScore, double decay, State *stack ) {

      
  //cerr << "Calling at depth " << depth << endl;
  if (status.me.isDead()) // try to avoid dying
    return -1000;
  
  int RR = status.me.pos.row, CC = status.me.pos.col;
  if (stuck[RR][CC])
    return -1000 + (reachable_gold[RR][CC] * 100.0) *
      (TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
  
    int other = 0;
    if (!status.you.isDead()) {
      if ( (status.board.visible(status.you.pos) == REMOVED_BRICK) && ( (status.board[status.you.pos] < 180 ) || (status.board[status.you.pos] > 204) ) )
	other += 300;
      other += (reachable_gold[RR][CC] * 100.0) * 
	(TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
    }
    for (unsigned int i = 0; i < status.enemies.size(); i++) {
      if (!status.enemies[i].isDead())
	if ( (status.board.visible(status.enemies[i].pos) == REMOVED_BRICK) && ( (status.board[status.enemies[i].pos] < 180 ) || (status.board[status.enemies[i].pos] > 204) ) )
	  other += 20;
    }

    int newScore = status.me.score + other;

    double scoreDelta = decay * (newScore - oldScore);

    //double temp_score = decay * (status.me.score + other);
    decay *= 0.9;

  if (depth >= max_depth || turns_remaining <= 0) {
    if (!status.me.isDead() && status.fallingDude(status.me)) {
      //      cerr << "Falling : " << status.me << ", depth = " << depth << endl;
      //      cerr << "Board : " << (int)status.board.visible(status.me.pos) << endl;
      //      cerr << "Below : " << (int)status.board.visible(status.me.pos.row+1, status.me.pos.col) << endl;
      //      cerr << "board : " << endl;
      //      cerr << status.board << endl;
      return boundedLookaheadScoreNewDecay(g, status.getNewState(g, DOWN, NONE, stack[depth-1]),
				   depth+1, max_depth, turns_remaining-1, newScore, currScore + scoreDelta, decay, stack);
    }

    return currScore + scoreDelta;

  }
  vector<cmd> commands;
  getViableCommands(status, status.me, commands);

  int score = -999999998;
  
  //State *newStatus = new State();
  //const double decay = 0.9;
  for (unsigned int i = 0; i != commands.size(); i++) {
    int tempscore = decay * boundedLookaheadScoreNewDecay(g, status.getNewState(g, commands[i], NONE, stack[depth-1]), depth+1, max_depth, turns_remaining-1,
		        newScore, currScore + scoreDelta, decay, stack);
    if (tempscore > score)
      score = tempscore;
  }
  
  return score;
  
}

double Lookahead::boundedLookaheadScore( const Game &g, const State &status,
					 int depth, int max_depth,
					 int turns_remaining, State *stack ) {
  if (status.me.isDead()) // try to avoid dying
    return -1000;

  int RR = status.me.pos.row, CC = status.me.pos.col;
  if (stuck[RR][CC])
    return -1000 + (reachable_gold[RR][CC] * 100.0) *
      (TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
  
  if (depth >= max_depth || turns_remaining <= 0) {
    if (!status.me.isDead() && status.fallingDude(status.me)) {
      return boundedLookaheadScore(g, status.getNewState(g, DOWN, NONE, stack[depth+1]),
				   depth+1, max_depth, turns_remaining-1, stack);
    }
    int other = 0;
    if (!status.you.isDead()) {
      if ( (status.board.visible(status.you.pos) == REMOVED_BRICK) && ( (status.board[status.you.pos] < 180 ) || (status.board[status.you.pos] > 204) ) )
	other += 300;
      other += (reachable_gold[RR][CC] * 100.0) * 
	(TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
    }
    for (unsigned int i = 0; i < status.enemies.size(); i++) {
      if (!status.enemies[i].isDead())
	if ( (status.board.visible(status.enemies[i].pos) == REMOVED_BRICK) && ( (status.board[status.enemies[i].pos] < 180 ) || (status.board[status.enemies[i].pos] > 204) ) )
	  other += 20;
    }

    return status.me.score + other;
  }
  vector<cmd> commands;
  getViableCommands(status, status.me, commands);
  
  int score = -999999998;
  
  //State *newStatus = new State();
  const double decay = 0.9;
  for (unsigned int i = 0; i != commands.size(); i++) {
    int tempscore = decay * boundedLookaheadScore(g, status.getNewState(g, commands[i], NONE, stack[depth-1]), depth+1, max_depth, turns_remaining-1, stack);
    if (tempscore > score)
      score = tempscore;
  }
  
  return score;
  
}

int Lookahead::boundedLookahead( const Game &g, const State &status,
				 int maxDepth, int turns_remaining,
				 vector<cmd> &potentials ) {
     
  potentials.clear();
    
  State *stack = new State[maxDepth+HEIGHT+1];
  
  cmd order[] = {DIG_LEFT, LEFT, DIG_RIGHT, RIGHT, UP, DOWN, NONE};
  
  vector<cmd> commands;
  getViableCommands(status, status.me, commands);
 

  vector<cmd> notdead;

  //cerr << "Commands: ";
  //for (int i = 0; i < commands.size(); i++) {
  //	cerr << names[commands[i]] << " ";
  //}
  //cerr << endl;
  
  double bestScore = -999999999;
  double minScore = 10000000;  

  State *newStatus = new State();
  //cerr << "Starting" << endl;
  //
  //
  
    int other = 0;
    if (!status.you.isDead())
      if ( (status.board.visible(status.you.pos) == REMOVED_BRICK) && ( (status.board[status.you.pos] < 180 ) || (status.board[status.you.pos] > 204) ) )
	other += 300;
    for (unsigned int i = 0; i < status.enemies.size(); i++) {
      if (!status.enemies[i].isDead())
	if ( (status.board.visible(status.enemies[i].pos) == REMOVED_BRICK) && ( (status.board[status.enemies[i].pos] < 180 ) || (status.board[status.enemies[i].pos] > 204) ) )
	  other += 20;
    }

    int newScore = status.me.score + other;


  for (int i = 0; i != 7; i++) {
    if ( find(commands.begin(), commands.end(), order[i]) == commands.end() )
      continue;
    //double tempscore = boundedLookaheadScore(g, status.getNewState(g, order[i], NONE, *newStatus), 1, maxDepth, turns_remaining, stack);
    double tempscore = boundedLookaheadScoreNewDecay(g, status.getNewState(g, order[i], NONE, *newStatus), 1, maxDepth, turns_remaining, newScore, 0.0, 1.0, stack);

    if (tempscore >= 0.0)
	    notdead.push_back(order[i]);

    //cerr << "Tempscore for move " << names[order[i]] << " = " << tempscore << endl;
    minScore = min(minScore, tempscore);
    if (tempscore > bestScore) {
      potentials.clear();
      bestScore = tempscore;
      potentials.push_back(order[i]);
    } else if (tempscore == bestScore) {
      potentials.push_back(order[i]);
    }
  }

  if (bestScore < 1) {
	  potentials.clear();
	  for (size_t i = 0; i < notdead.size(); i++)
		  potentials.push_back(notdead[i]);
  }

  if (potentials.size() == 0)
	  potentials.push_back(NONE);
  
  delete newStatus;
  delete [] stack;
  //cerr << "Choice for move: " << names[order[besti]] << " = " << bestScore << endl;
  //return order[besti];
  return (int)floor(bestScore - minScore);
}

/***************************************
 * Darcy's changes -- the all but dead values.
 */

double Lookahead::boundedLookaheadScoreABD( const Game &g, const State &status,
					 int depth, int max_depth,
					 int turns_remaining, State *stack ) {
  if (status.me.isDead()) // try to avoid dying
    return -1000;

  int RR = status.me.pos.row, CC = status.me.pos.col;
  if (stuck[RR][CC])
    return -1000 + (reachable_gold[RR][CC] * 100.0) *
      (TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
  
  if (depth >= max_depth || turns_remaining <= 0) {
    if (!status.me.isDead() && status.fallingDude(status.me)) {
      //      cerr << "Falling : " << status.me << ", depth = " << depth << endl;
      //      cerr << "Board : " << (int)status.board.visible(status.me.pos) << endl;
      //      cerr << "Below : " << (int)status.board.visible(status.me.pos.row+1, status.me.pos.col) << endl;
      //      cerr << "board : " << endl;
      //      cerr << status.board << endl;
      return boundedLookaheadScoreABD(g, status.getNewState(g, DOWN, NONE, stack[depth+1]),
				   depth+1, max_depth, turns_remaining-1, stack);
    }
    int other = 0;
    if (!status.you.isDead()) {
      if ( (status.board.visible(status.you.pos) == REMOVED_BRICK) && ( (status.board[status.you.pos] < 180 ) || (status.board[status.you.pos] > 204) ) )
	other += 300;
      other += (reachable_gold[RR][CC] * 100.0) * 
	(TURNS_REMAINING + GOLD_RESPAWN_TIME - 1) / GOLD_RESPAWN_TIME;
    }
    for (unsigned int i = 0; i < status.enemies.size(); i++) {
      if (!status.enemies[i].isDead())
	if ( (status.board.visible(status.enemies[i].pos) == REMOVED_BRICK) && ( (status.board[status.enemies[i].pos] < 180 ) || (status.board[status.enemies[i].pos] > 204) ) )
	  other += 20;
    }

    return status.me.score + other;
  }
  vector<cmd> commands;
  getViableCommands(status, status.me, commands);

  int score = -999999998;

  //State *newStatus = new State();
  const double decay = 1;
  for (unsigned int i = 0; i != commands.size(); i++) {
    int tempscore = decay * boundedLookaheadScoreABD(g, status.getNewState(g, commands[i], NONE, stack[depth-1]), depth+1, max_depth, turns_remaining-1, stack);
    if (tempscore > score)
      score = tempscore;
  }

  return score;

}


int Lookahead::boundedLookaheadABD( const Game &g, const State &status,
				 int maxDepth, int turns_remaining,
				 int threshold, vector<cmd> &potentials ) {

  potentials.clear();

  State *stack = new State[maxDepth+HEIGHT+1];

  cmd order[] = {DIG_LEFT, LEFT, DIG_RIGHT, RIGHT, UP, DOWN, NONE};

  vector<cmd> commands;
  getViableCommands(status, status.me, commands);

  //cerr << "Commands: ";
  //for (int i = 0; i < commands.size(); i++) {
  //	cerr << names[commands[i]] << " ";
  //}
  //cerr << endl;

  double bestScore = -999999999;
  double minScore = 10000000;

  State *newStatus = new State();

  for (int i = 0; i != 7; i++) {
    if ( find(commands.begin(), commands.end(), order[i]) == commands.end() )
      continue;
    double tempscore = boundedLookaheadScoreABD(g, status.getNewState(g, order[i], NONE, *newStatus), 1, maxDepth, turns_remaining, stack);
    //    cerr << "Tempscore for move " << names[order[i]] << " = " << tempscore << endl;
    minScore = min(minScore, tempscore);
    if (tempscore > threshold - 1000)
    	potentials.push_back(order[i]);

    if (tempscore > bestScore) {
      bestScore = tempscore;
    }
  }

  delete newStatus;
  delete [] stack;
  //cerr << "Choice for move: " << names[order[besti]] << " = " << bestScore << endl;
  //return order[besti];
  return (int)floor(bestScore - minScore);
}


/////////////////////////////////////////////////////////////////////////
// Utilities
/////////////////////////////////////////////////////////////////////////
bool equivalent(const Player &e1, const Player &e2) {
  return (e1.pos == e2.pos) && (abs(e1.score - e2.score) < 10) && (e1.mason_count == e2.mason_count);
}

bool equivalent(const Enemy &e1, const Enemy &e2) {
  return (e1.pos == e2.pos) && (e1.master == e2.master);
}

bool equivalent(const State &s1, const State &s2, bool verbose) {
  if (s1.turn != s2.turn) {
    if (verbose)
      cerr << "Failed to match turn" << endl;
    return false;
  }
  
  for (int row = 0; row < HEIGHT; row++)
    for (int col = 0; col < WIDTH; col++) {
      Position p(row, col);
      if (s1.board.visible(p) != s2.board.visible(p)) {
	if (verbose)
	  cerr << "Failed to match at position " << p << endl;
	return false;
      }
    }
  
  if ( !equivalent(s1.me, s2.me) ) {
    if (verbose) {
      cerr << "Failed to match me" << endl;
      cerr << "s1.me = " << s1.me << endl;
      cerr << "s2.me = " << s2.me << endl;
    }
    return false;
  }
  
  if ( !equivalent(s1.you, s2.you) ) {
    if (verbose)
      cerr << "Failed to match you" << endl;
    return false;
  }
  
  for (unsigned int i = 0; i < s1.enemies.size(); i++) {
    if ( !equivalent(s1.enemies[i], s2.enemies[i]) ) {
      if (verbose)
	cerr << "Failed to match enemy " << i << endl;
      
      return false;
    }
  }
  
  return true;
}
