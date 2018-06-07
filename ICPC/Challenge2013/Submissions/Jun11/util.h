#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES__
#define __UOFL_ICPC_CHALLENGE_UTILITIES__

#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <string>
#include <stack>
#include <utility>
#include <cassert>
#include <map>
using namespace std;

// **************************************************
//   GLOBAL VARIABLES
// **************************************************

const int HEIGHT = 16;
const int WIDTH = 25;

const double PI = acos(-1.0);

const int oo = 1337;

enum cmd { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };
enum Item { EMPTY , LADDER , BRICK , GOLD , REMOVED_BRICK };

const int di[] = {-1, 1, 0, 0, 1, 1, 0};
const int dj[] = { 0, 0,-1, 1,-1, 1, 0};
const int dijkstra_dist[] = { 1, 1, 1, 1, 3, 3, 1 };
const cmd delta[] = { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };
const cmd opp[] = { DOWN, UP, RIGHT, LEFT, DIG_RIGHT, DIG_LEFT, NONE };
typedef pair<int,int> pii;

int CURRENT_TURN, NUMBER_OF_TURNS;
int NUMBER_OF_ENEMIES;
int respawn_row,respawn_col;
int alive_time; // Time that you have been alive for

const int PLAYER_RESPAWN_TIME = 50;
const int GOLD_RESPAWN_TIME = 150;
const int BRICK_RESPAWN_TIME = 25;
const int ENEMY_RESPAWN_TIME = 25;

vector<int> gold_respawns_in; // -1 == there now, otherwise, self-explanitory
vector<int> enemy_respawns_in; // Same as other two
int brick_respawns_in[HEIGHT][WIDTH]; // Same system as gold and enemy.
bool opp_dug_me[HEIGHT][WIDTH]; // Only means something where there is a REMOVED_BRICK

// **************************************************
//   FUNCTIONS YOU CAN USE
// **************************************************

// Input / Output Functions
//  read_world      initializes the program at the very beginning
//  read_turn       reads in the information about the current turn
//  output_command  tells your player to perform that command
//  output_command2 is the same as output_command, and outputs our command
//                   to output so you can see what is happening as well
// Misc
//  valid_indices   determines if (r,c) are valid coordinates
//  board           gives the item is on the current board at (r,c)
//  valid_moves     gives a vector of valid moves you can make
//  valid_moves2    gives a vector of valid moves your opponent can make
//  can_hammer      tells you if you can hammer right now
//  next_hammer     tells you the next time you are allowed to hammer
//  alive           tells you if you are alive
//  falling         tells you if you are currently falling
//  curr_loc        tells you the current location of yourself
//  closest_gold    tells you how far away the closest gold is (and how
//                   to get there)
//  Goto            gives a command to output that gets you to location (r,c)
//                   in the least number of turns
//  end_up          tells you which square you will end up at if you do a move.
//                   It follows through holes but assumes that falls are 
//                   instanenous.

void read_world();
void read_turn();
void output_command( cmd C );
void output_command2( cmd C );

bool valid_indices(int r,int c);
Item board(int row,int col);

int distance(int r1,int c1,int r2,int c2);

void sort_enemies();

vector<cmd> valid_moves();
vector<cmd> valid_moves2();

vector<cmd> nextmove_dont_use_me_use_Goto_instead[HEIGHT][WIDTH][HEIGHT][WIDTH];

bool can_hammer();
int next_hammer();
bool alive();
bool falling();
void curr_loc(int& r,int & c);

cmd Goto(int r,int c);
pii end_up(cmd C);

pair<cmd,int> suicide_move();

// **************************************************
//   IMPLEMENTATION OF THE FUNCTIONS (Change these at your own risk...)
// **************************************************

#include "enemy.h"
vector<Enemy> enemies,enemies2;

#include "board.h"
vector<Gold> golds;
vector<Gold> golds2;

#include "player.h"
Player P1,P2;
map<int,int> golds_in_scc, enemies_in_scc, scc_area;

Board origBoard,currBoard;


void output_command( cmd C ){
  // Add any error checking you need here.
  //  For example, should we ever go into a hole that is about to close?
  const string COMMAND_NAME[] =
  { "TOP","BOTTOM","LEFT","RIGHT","DIG_LEFT","DIG_RIGHT","NONE" };
  cout << COMMAND_NAME[C] << endl;
}

void output_command2( cmd C ){
  // Add any error checking you need here.
  //  For example, should we ever go into a hole that is about to close?
  const string COMMAND_NAME[] =
  { "TOP","BOTTOM","LEFT","RIGHT","DIG_LEFT","DIG_RIGHT","NONE" };
  cerr << COMMAND_NAME[C] << " -- " << int(C) << endl;
  cout << COMMAND_NAME[C] << endl;
}

bool valid_indices(int ROW,int COL){
  return (0 <= ROW && ROW < HEIGHT && 0 <= COL && COL < WIDTH);
}

void read_world(){
  cin >> NUMBER_OF_TURNS;
  
  origBoard.read(); // Read in the board
  origBoard.init(0,0,0);
  origBoard.initSCC();

  alive_time = -1;

  P1.read_and_init(); // Read in location of player 1 (Me)
  P2.read_and_init(); // Read in location of player 2 (Opponent)
  
  respawn_row = P1.respawn_row;
  respawn_col = P1.respawn_col;

  cin >> NUMBER_OF_ENEMIES;
  for(int i=0;i<NUMBER_OF_ENEMIES;i++){
    Enemy E;
    E.read_and_init(i);
    enemies.push_back(E);
    enemies_in_scc[origBoard.scc_index(E.row, E.col)]++;
  }

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      scc_area[origBoard.scc_index(i,j)]++;
    
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      visited_[i][j] = false;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(origBoard.itemAt(i,j) == GOLD){
	Gold G;
	G.row = i;
	G.col = j;
	G.id = golds.size();
	golds.push_back(G);
	gold_respawns_in.push_back(-1);
	golds_in_scc[origBoard.scc_index(i, j)]++;
      }

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      brick_respawns_in[i][j] = -1;
  
}

bool any_enemies(){
  return enemies.size() && enemies[0].alive &&
    board(enemies[0].row,enemies[0].col) != REMOVED_BRICK;
}

int dist_to_enemy(const Enemy& E,bool youGoingToThem, int r, int c){
  bool alive = E.alive && board(E.row,E.col) != REMOVED_BRICK;
  int extra = 0;

  // I intentionally have this as 30, not 25. You can replace it with a more accurate value
  //   if applicable...
  if(!alive)
    extra = (board(E.row,E.col) == REMOVED_BRICK ? 30 : enemy_respawns_in[E.id]);
  
  int eRow = alive ? E.row : E.respawn_row;
  int eCol = alive ? E.col : E.respawn_col;

  if(youGoingToThem){
    return distance(r,c,eRow,eCol) + extra;
  } else {
    return distance(eRow,eCol,r,c) + extra;
  }
}

int dist_to_enemy(const Enemy& E,bool youGoingToThem){
  return dist_to_enemy(E, youGoingToThem, P1.row, P1.col);
}


bool enemy_dist_to_you(const Enemy& e1,const Enemy& e2){
  return dist_to_enemy(e1,false) < dist_to_enemy(e2,false);
}

bool enemy_dist_to_them(const Enemy& e1,const Enemy& e2){
  return dist_to_enemy(e1,true) < dist_to_enemy(e2,true);
}

bool by_id(const Enemy& e1,const Enemy& e2){
  return e1.id < e2.id;
}

int dist_to_gold(const Gold& G){
  int d1 = distance(P1.row,P1.col,G.row,G.col);
  int d2 = (board(G.row,G.col) == GOLD ? 0 : gold_respawns_in[G.id]);
  return max(d1,d2);    
}

int dist_to_goldP2(const Gold& G){
  int d1 = distance(P2.row,P2.col,G.row,G.col);
  int d2 = (board(G.row,G.col) == GOLD ? 0 : gold_respawns_in[G.id]);
  return max(d1,d2);    
}

bool gold_dist(const Gold& G1,const Gold& G2){
  int P1G1 = dist_to_gold(G1);
  int P1G2 = dist_to_gold(G2);
  int P2G1 = dist_to_goldP2(G1);
  int P2G2 = dist_to_goldP2(G2);
  
  bool closer1 = P1G1 <= P2G1;
  bool closer2 = P1G2 <= P2G2;

  if (closer1 != closer2) {
    return closer1;
  }

  return dist_to_gold(G1) < dist_to_gold(G2);
}

bool gold_dist2(const Gold& G1,const Gold& G2){
  int player_scc = origBoard.scc_index(P1.row, P1.col);
  //  int opp_scc = currBoard.scc_index(P2.row, P2.col);
  int G1_scc = origBoard.scc_index(G1.row, G1.col);
  int G2_scc = origBoard.scc_index(G2.row, G2.col);

  // if current SCC still has gold, perhaps we should stay
  if (G1_scc != G2_scc) {
    if (player_scc == G1_scc && golds_in_scc[G1_scc]) {
      return true;
    } else if (player_scc == G2_scc && golds_in_scc[G2_scc]) {
      return false;
    }
  }
  
  // we want to maximize:
  //   (expected gold) / how far to get their / number of enemies
  return dist_to_gold(G1) * golds_in_scc[G2_scc] * enemies_in_scc[G1_scc] <
    dist_to_gold(G2) * golds_in_scc[G1_scc] * enemies_in_scc[G2_scc];
}

void read_turn(){
  // **************************************************************
  //  Read in from them (Probably shouldn't touch this)
  // **************************************************************

  int curr_turn;
  cin >> curr_turn;
  if(curr_turn == -1) return; // End of game (we must have missed a turn...)
  currBoard.read();
  P1.read();
  P2.read();

  if(P1.row == -1) alive_time = -1;
  else if(alive_time == -1) alive_time = 0;
  else alive_time++;

  // **************************************************************
  //  Everything to do with bricks (must be done before init)
  // **************************************************************
  
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      if(board(i,j) == REMOVED_BRICK){
	if(brick_respawns_in[i][j] == -1){
	  brick_respawns_in[i][j] = 25;
	  opp_dug_me[i][j] =
	    (P2.row+1 == i && P2.col+1 == j) || 
	    (P2.row+1 == i && P2.col-1 == j); 
	}
	else brick_respawns_in[i][j]--;
      }
      if(board(i,j) == BRICK) brick_respawns_in[i][j] = -1;
    }

  currBoard.init(P1.row,P1.col,P1.dig_delay);
  
  if(P1.alive) visited_[P1.row][P1.col] = true;
  if(P2.alive) visited_[P2.row][P2.col] = true;

  // **************************************************************
  //  Everything to do with gold
  // **************************************************************

  golds_in_scc.clear();
  for(size_t i=0;i<golds.size();i++){
    if(board(golds[i].row,golds[i].col) == GOLD){
      gold_respawns_in[golds[i].id] = -1;
      golds_in_scc[origBoard.scc_index(golds[i].row, golds[i].col)]++;
    } else {
      if(gold_respawns_in[golds[i].id] == -1)
	gold_respawns_in[golds[i].id] = GOLD_RESPAWN_TIME;
      else
	gold_respawns_in[golds[i].id]--;
      if (gold_respawns_in[golds[i].id] <= 2) {
	golds_in_scc[origBoard.scc_index(golds[i].row, golds[i].col)]++;
      }
    }
  }
  sort(golds.begin(),golds.end(),gold_dist);
  golds2 = golds;
  if (alive())
    sort(golds2.begin(),golds2.end(),gold_dist2);
  
  // **************************************************************
  //  Everything to do with enemies
  // **************************************************************

  // Deal with enemies...
  sort(enemies.begin(),enemies.end(),by_id);
  for(int i=0;i<NUMBER_OF_ENEMIES;i++) enemies[i].read_and_update();
  enemies2 = enemies;
  sort(enemies.begin(),enemies.end(),enemy_dist_to_you);
  sort(enemies2.begin(),enemies2.end(),enemy_dist_to_them);
}

Item board(int row,int col){
  return currBoard.itemAt(row,col);
}

vector<cmd> valid_moves() { return P1.validMoves(currBoard); }
vector<cmd> valid_moves2(){ return P2.validMoves(currBoard); }

void curr_loc(int& r,int & c){ r = P1.row; c = P1.col; }

bool alive(){ return P1.alive; }
bool can_hammer(){
  vector<cmd> v = valid_moves();
  for(size_t i=0;i<v.size();i++)
    if(v[i] == DIG_LEFT || v[i] == DIG_RIGHT)
      return true;
  return false;
}

bool falling(){
  return P1.alive && !currBoard.canStand(P1.row,P1.col);
}

// Goes from (r1,c1) to (r2,c2)
int distance(int r1,int c1,int r2,int c2){
  if(!valid_indices(r1,c1) || !valid_indices(r2,c2)) return oo;
  
  if(r1 == P1.row && c1 == P1.col)
    return currBoard.dist_with_fall[r2][c2];
  return currBoard.dist_no_fall[r1][c1][r2][c2];
}

// This returns a vector of all commands that will get you
//     (r1,c1) --> (r2,c2)
// the quickest
vector<cmd> Goto(int r1,int c1,int r2,int c2){
  int dist = distance(r1,c1,r2,c2);
  if(dist == oo) return vector<cmd>(1,NONE);
  if(dist == 0)  return vector<cmd>(1,NONE);

  return nextmove_dont_use_me_use_Goto_instead[r1][c1][r2][c2];
}

void end_up(cmd C, int &r, int &c)
{
  curr_loc(r, c);

  vector<cmd> list = currBoard.valid_moves(r, c, false);
  if (find(list.begin(), list.end(), C) == list.end()) {
    C = NONE;
  }

  if (C == NONE) return;
  r += di[C];
  c += dj[C];

  while (!currBoard.canStand(r, c)) r++;
  return;
}

bool CouldFallOnZombie(int r, int c) {
  if (!valid_indices(r, c)) return false;
  if (board(r, c) != EMPTY && board(r,c) != REMOVED_BRICK)
    return false;

  int i;
  for (i = r; i < HEIGHT; ++i) {
    if (board(i, c) == BRICK || board(i, c) == LADDER) break;
  }
  r = i+1;
  // Is there a zombie close by?
  double dist = 1e18;
  for (int i = 0; i < (int)enemies.size(); ++i) {
    // If this enemy is actually stuck in a hole, then ignore him.
    if (board(enemies[i].row, enemies[i].col) == REMOVED_BRICK)
      continue;
    double d = (r-enemies[i].row)*(r-enemies[i].row) +
      (c-enemies[i].col)*(c-enemies[i].col);
    if (d < dist) dist = d;
  }

  return sqrt(dist) < 5; // tune this ...
}

bool WillGetStuck(cmd C) {
  int r,c;
  end_up(C,r,c);

  return
    (!valid_indices(r, c-1) || board(r, c-1) == BRICK) &&
    (!valid_indices(r, c+1) || board(r, c+1) == BRICK);
}

// Returns what move to take and the estimated suicide time...
pair<cmd,int> suicide_move(){
  int m1 = oo,  m2 = oo;
  cmd c1 = NONE,c2 = NONE;

  vector<cmd> moves = valid_moves();
  
  // Method 1: Run straight to enemy
  if(!enemies2.empty()){
    vector<cmd> v = Goto(P1.row,P1.col,enemies2[0].row,enemies2[0].col);
    if(!v.empty()){
      // As you get close, they will walk towards you. If you are their master,
      //  then it will reduce it by 2. If the opponent is their master, then it
      //  will reduce it by 4.
      int fudge_factor = (enemies2[0].master == 1 ? 4 : 2);
      m1 = max(0,distance(P1.row,P1.col,enemies2[0].row,enemies2[0].col) - fudge_factor);
      c1 = v[0];
    }    
  }
  
  // Method 2: Dig a hole and jump in
  //  Method 2(a): Am I already in a hole?
  if(board(P1.row,P1.col) == REMOVED_BRICK) return make_pair(NONE,brick_respawns_in[P1.row][P1.col]);
  
  //  Method 2(b): Is the hole already dug?
  if(board(P1.row+1,P1.col-1) == REMOVED_BRICK){
    int val = brick_respawns_in[P1.row+1][P1.col-1] + 2;
    if(val < m2){
      c2 = LEFT;
      m2 = val;
    }
  }
  if(board(P1.row+1,P1.col+1) == REMOVED_BRICK){
    int val = brick_respawns_in[P1.row+1][P1.col+1] + 2;
    if(val < m2){
      c2 = RIGHT;
      m2 = val;
    }
  }
  
  //  Method 2(c): I must dig the hole...
  for(size_t i=0;i<moves.size();i++)
    if(moves[i] == DIG_LEFT || moves[i] == DIG_RIGHT){
      int val = 25 + 3;
      if(val < m2){
	c2 = moves[i];
	m2 = val;
      }
      break;
    }
    
  // Which one is better?
  if(c1 == NONE) return make_pair(c2,m2);
  if(c2 == NONE) return make_pair(c1,m1);
  return (m1 < m2 ? make_pair(c1,m1) : make_pair(c2,m2));
}

#endif
