#ifndef __UOFL_ICPC_CHALLENGE_GLOBALS__
#define __UOFL_ICPC_CHALLENGE_GLOBALS__

#define FINAL_VERSION
#define NDEBUG

#include <utility>
#include <vector>
#include <map>
#include <cmath>
#include <string>

using namespace std;

/* ============================================
     Typedefs
============================================= */
typedef unsigned char uchar;
typedef pair<int,int> pii;
typedef uchar Item;

/////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////


const int HEIGHT = 16;
const int WIDTH = 25;

const uchar EMPTY = '.';
const uchar LADDER = 'H';
const uchar BRICK = '=';
const uchar REMOVED_BRICK = '-';
const uchar GOLD = '*';

const double PI = acos(-1.0);

const int oo = 1337;

enum cmd { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };

const string names[] =  {"UP", "DOWN", "LEFT", "RIGHT",
			 "DIG_LEFT", "DIG_RIGHT", "NONE" };
const string gameNames[] =  {"TOP", "BOTTOM", "LEFT", "RIGHT",
			     "DIG_LEFT", "DIG_RIGHT", "NONE" };

const int di[] = {-1, 1, 0, 0, 1, 1, 0};
const int dj[] = { 0, 0,-1, 1,-1, 1, 0};
const int dijkstra_dist[] = { 1, 1, 1, 1, 3, 3, 1 };
const cmd delta[] = { UP, DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };
const cmd opp[] = { DOWN, UP, RIGHT, LEFT, DIG_RIGHT, DIG_LEFT, NONE };

const int PLAYER_RESPAWN_TIME = 50;
const int GOLD_RESPAWN_TIME = 150;
const int BRICK_RESPAWN_TIME = 25;
const int ENEMY_RESPAWN_TIME = 25;

/////////////////////////////////////////////////////////////////////////
// Global Variables
/////////////////////////////////////////////////////////////////////////

extern int CURRENT_TURN, NUMBER_OF_TURNS, TURNS_REMAINING;
extern int NUMBER_OF_ENEMIES;
extern int respawn_row,respawn_col;
extern int alive_time; // Time that you have been alive for

extern vector<cmd> nextmove_dont_use_me_use_Goto_instead[HEIGHT][WIDTH][HEIGHT][WIDTH];
//extern map<int,int> golds_in_scc, enemies_in_scc, scc_area;

extern int dist_orig_board_reversible[HEIGHT][WIDTH][HEIGHT][WIDTH];
extern int dist_orig_board_with_dig[HEIGHT][WIDTH][HEIGHT][WIDTH];
extern int dist_curr_board_no_dig[HEIGHT][WIDTH][HEIGHT][WIDTH];
extern int dist_curr_board_with_dig[HEIGHT][WIDTH];

extern bool stuck[HEIGHT][WIDTH];
extern int reachable_gold[HEIGHT][WIDTH];
extern int nearest_gold_to_respawn;

//extern bool SCC_leaf[HEIGHT*WIDTH];
extern bool can_stuck;

#endif
