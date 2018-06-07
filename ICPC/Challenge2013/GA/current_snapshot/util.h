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
#include <cmath>
using namespace std;

#include "globals.h"
#include "classes.h"


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
void read_turn(const string &move);
string output_command( cmd C );
string output_command2( cmd C );

bool valid_indices(int r,int c);
Item board(int row,int col);

int distance(int r1,int c1,int r2,int c2);

vector<cmd> valid_moves();
vector<cmd> valid_moves2();

bool can_hammer();
int next_hammer();          // NOT IMPLEMENTED
bool alive();
bool falling();
void curr_loc(int& r,int & c);

pair<cmd,int> suicide_move();

cmd getCommand(const string &x);


//////////////////////////////////////////////////////////////////////////////

bool any_enemies();

int dist_to_enemy(const Enemy& E,bool youGoingToThem, int r, int c);
int dist_to_enemy(const Enemy& E,bool youGoingToThem);

bool enemy_dist_to_you(const Enemy& e1,const Enemy& e2);
bool enemy_dist_to_them(const Enemy& e1,const Enemy& e2);

int dist_to_gold(const Gold& G);
int dist_to_goldP2(const Gold& G);

bool gold_dist(const Gold& G1,const Gold& G2);
bool gold_dist2(const Gold& G1,const Gold& G2);



// This returns a vector of all commands that will get you
//     (r1,c1) --> (r2,c2)
// the quickest
vector<cmd> Goto(int r1,int c1,int r2,int c2);

void end_up(cmd C, int &r, int &c);

bool CouldFallOnZombie(int r, int c);

bool WillGetStuck(cmd C);


// 0 -- Do not commit suicide.
// 1 -- May want to commit suicide in the next 5 (ish) moves (not now).
// 2 -- I want to commit suicide unless you have something you really want.
// 3 -- I need to commit now. No choice.
int level_of_suicide();

void BFS__(Board& B,int dist[HEIGHT][WIDTH],int r,int c);
void BFS_F(Board& B,int dist[HEIGHT][WIDTH],int r,int c);
void BFSDF(Board& B,int dist[HEIGHT][WIDTH],int r,int c,
	   int delay,bool update_nextmove);
void update_and_add(PQ& pq,vector<cmd> moves[HEIGHT][WIDTH],
		    int dist[HEIGHT][WIDTH],
		    int dist2[HEIGHT][WIDTH][11],int nDist,
		    int nI,int nJ,int nD,cmd nC);

#endif
