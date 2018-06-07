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

const int GOLD_RESPAWN_TIME = 150;
vector<int> gold_respawns_in;

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

vector<cmd> nextmove[HEIGHT][WIDTH][HEIGHT][WIDTH];

bool can_hammer();
int next_hammer();
bool alive();
bool falling();
void curr_loc(int& r,int & c);

pair<cmd,int> closest_gold();

cmd Goto(int r,int c);
pii end_up(cmd C);

// **************************************************
//   IMPLEMENTATION OF THE FUNCTIONS (Change these at your own risk...)
// **************************************************

#include "enemy.h"
vector<Enemy> enemies,enemies2;

#include "board.h"
vector<Gold> golds;
Board origBoard,currBoard;

#include "player.h"
Player P1,P2;

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
  origBoard.init(0);
  
  P1.read_and_init(); // Read in location of player 1 (Me)
  P2.read_and_init(); // Read in location of player 2 (Opponent)
  
  cin >> NUMBER_OF_ENEMIES;
  for(int i=0;i<NUMBER_OF_ENEMIES;i++){
    Enemy E;
    E.read_and_init(i);
    enemies.push_back(E);
  }
  
  sort_enemies();
  
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      visited_[i][j] = false;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(origBoard.A[i][j] == GOLD){
	Gold G;
	G.row = i;
	G.col = j;
	G.id = golds.size();
	golds.push_back(G);
	gold_respawns_in.push_back(-1);
      }
}

bool any_enemies(){
  return enemies.size() && enemies[0].alive &&
    board(enemies[0].row,enemies[0].col) != REMOVED_BRICK;
}

int dist_to_enemy(const Enemy& E,bool youGoingToThem){
  bool alive = E.alive && board(E.row,E.col) != REMOVED_BRICK;
  int extra = 0;
  if(!alive)
    extra = (board(E.row,E.col) != REMOVED_BRICK ? 25 : enemy_respawns_in[E.id]);
  
  int eRow = alive ? E.row : E.respawn_row;
  int eCol = alive ? E.col : E.respawn_col;

  if(youGoingToThem){
    return distance(P1.row,P1.col,eRow,eCol) + extra/2;
  } else {
    return distance(eRow,eCol,P1.row,P1.col) + extra/2;
  }
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

bool gold_dist(const Gold& G1,const Gold& G2){
  return dist_to_gold(G1) < dist_to_gold(G2);
}

void sort_enemies(){
  enemies2 = enemies;
  sort(enemies.begin(),enemies.end(),enemy_dist_to_you);
  enemies2 = enemies;
  sort(enemies2.begin(),enemies2.end(),enemy_dist_to_them);
}

void read_turn(){
  int curr_turn;
  cin >> curr_turn;
  currBoard.read();
  P1.read();
  P2.read();
  currBoard.init(P1.dig_delay);
  
  if(P1.alive) visited_[P1.row][P1.col] = true;
  if(P2.alive) visited_[P2.row][P2.col] = true;

  for(size_t i=0;i<golds.size();i++){
    if(board(golds[i].row,golds[i].col) == GOLD){
      gold_respawns_in[golds[i].id] = -1;
    } else {
      if(gold_respawns_in[golds[i].id] == -1)
	gold_respawns_in[golds[i].id] = GOLD_RESPAWN_TIME;
      else
	gold_respawns_in[golds[i].id]--;
    }
  }

  sort(golds.begin(),golds.end(),gold_dist);
  
  sort(enemies.begin(),enemies.end(),by_id);
  for(int i=0;i<NUMBER_OF_ENEMIES;i++) enemies[i].read_and_update();
  sort_enemies();
}

Item board(int row,int col){
  if(!valid_indices(row,col)) return EMPTY;
  return currBoard.A[row][col];
}

vector<cmd> valid_moves() { return P1.validMoves(currBoard); }
vector<cmd> valid_moves2(){ return P2.validMoves(currBoard); }

bool alive() { return P1.alive; }
bool can_hammer() { return alive() && next_hammer() == 0; }
int next_hammer() { return P1.dig_delay; }

void curr_loc(int& r,int & c){ r = P1.row; c = P1.col; }

bool falling(){
  return alive() && !currBoard.canStand(P1.row,P1.col);
}

// Goes from (r1,c1) to (r2,c2)
int distance(int r1,int c1,int r2,int c2){
  if(!valid_indices(r1,c1) || !valid_indices(r2,c2)) return oo;
  //currBoard.BFS_with_fall();
  return currBoard.dist_with_fall[r1][c1][r2][c2];
}

vector<cmd> Goto(int r1,int c1,int r2,int c2){
  int dist = distance(r1,c1,r2,c2);
  if(dist == oo) return vector<cmd>(1,NONE);
  if(dist == 0)  return vector<cmd>(1,NONE);
  
  vector<cmd> ans;
  vector<cmd> v = valid_moves();
  for(size_t k=0;k<v.size();k++){
     int nI = r1+di[v[k]], nJ = c1+dj[v[k]];
     if(!valid_indices(nI,nJ)) continue;
     if(dist - dijkstra_dist[v[k]] == currBoard.dist_with_fall[nI][nJ][r2][c2])
       ans.push_back(delta[v[k]]);
  }
  
  return ans;
}

cmd Goto(int r,int c){
  vector<cmd> v = Goto(P1.row,P1.col,r,c);
  if(v.empty()) return NONE;
  return v[0];
}

void end_up(cmd C, int &r, int &c)
{
  curr_loc(r, c);

  vector<cmd> list = currBoard.valid_moves(r, c, false);
  if (find(list.begin(), list.end(), C) == list.end()) {
    C = NONE;
  }

  if (C == NONE) return;
  if (C == UP) { if (r > 0) r--; return; }
  if (C == DOWN) { if (r < HEIGHT-1) r++; return; }

  if (C == LEFT) c--;
  else if (C == RIGHT) c++;

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

#endif
