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

enum cmd { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };
enum Item { EMPTY , LADDER , BRICK , GOLD , REMOVED_BRICK };

const int di[] = {-1, 1, 0, 0, 0, 0, 0};
const int dj[] = { 0, 0,-1, 1, 0, 0, 0};
const cmd delta[] = { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };

typedef pair<int,int> pii;

int CURRENT_TURN, NUMBER_OF_TURNS;
int NUMBER_OF_ENEMIES;

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

void read_world();
void read_turn();
void output_command( cmd C );
void output_command2( cmd C );

bool valid_indices(int r,int c);
Item board(int row,int col);

vector<cmd> valid_moves();
vector<cmd> valid_moves2();

bool can_hammer();
int next_hammer();
bool alive();
bool falling();
void curr_loc(int& r,int & c);

pair<cmd,int> closest_gold();

cmd Goto(int r,int c);

// **************************************************
//   IMPLEMENTATION OF THE FUNCTIONS (Change these at your own risk...)
// **************************************************

#include "board.h"
#include "player.h"
#include "enemy.h"

Board origBoard,currBoard;
Player P1,P2;
vector<Enemy> enemies;

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
  cerr << COMMAND_NAME[C] << endl;
  cout << COMMAND_NAME[C] << endl;
}

bool valid_indices(int ROW,int COL){
  return (0 <= ROW && ROW < HEIGHT && 0 <= COL && COL < WIDTH);
}

void read_world(){
  cin >> NUMBER_OF_TURNS;
  
  origBoard.read_and_init(); // Read in the board
  
  P1.read_and_init(); // Read in location of player 1 (Me)
  P2.read_and_init(); // Read in location of player 2 (Opponent)
  
  cin >> NUMBER_OF_ENEMIES;
  for(int i=0;i<NUMBER_OF_ENEMIES;i++){
    Enemy E;
    E.read_and_init();
    enemies.push_back(E);
  }
}

void read_turn(){
  int curr_turn;
  cin >> curr_turn;
  currBoard.read();
  P1.read();
  P2.read();
  for(int i=0;i<NUMBER_OF_ENEMIES;i++)
    enemies[i].read_and_update();

  for(int i=0;i<NUMBER_OF_ENEMIES;i++){
    int r = enemies[i].row, c = enemies[i].col;
    if(enemies[i].alive && board(r,c) == REMOVED_BRICK)
      currBoard.A[r][c] = BRICK;
  }
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

pair<cmd,int> closest_gold(){
  if(!alive()) return make_pair(NONE,0);
  vector<cmd> v = valid_moves();

  int r,c;
  curr_loc(r,c);

  currBoard.BFS_with_fall();
  
  int closest = 401; // Too large...
  int best_r=0,best_c=0;
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(board(i,j) == GOLD){
	if(currBoard.dist_with_fall[r][c][i][j] == -1) continue;
	if(closest > currBoard.dist_with_fall[r][c][i][j]){
	  closest = currBoard.dist_with_fall[r][c][i][j];
	  best_r = i;
	  best_c = j;
	}
      }
  return make_pair(Goto(best_r,best_c),closest);
}

pair<cmd,int> safest_gold(){
   if(near_gold(r,c) == 3){
      cout << "Enter here" << endl;
      return make_pair(LEFT,654);
   }
   if(
}

cmd Goto(int r,int c){
  int R,C;
  curr_loc(R,C);

  currBoard.BFS_with_fall();
  int dist = currBoard.dist_with_fall[R][C][r][c];
  if(dist == -1) return NONE;
  
  vector<cmd> v = valid_moves();
  for(size_t k=0;k<v.size();k++){
    int nI = R+di[v[k]], nJ = C+dj[v[k]];
    if(!valid_indices(nI,nJ)) continue;
    if(dist - 1 == currBoard.dist_with_fall[nI][nJ][r][c])
      return delta[v[k]];
  }
  
  return NONE;
}

#endif
