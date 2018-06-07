#include "globals.h"
#include "classes.h"

int CURRENT_TURN = 0, NUMBER_OF_TURNS = 0, TURNS_REMAINING = 0;
int NUMBER_OF_ENEMIES = 0;
int respawn_row = 0, respawn_col = 0;
int alive_time = 0;

vector<cmd> nextmove_dont_use_me_use_Goto_instead[HEIGHT][WIDTH][HEIGHT][WIDTH];
map<int,int> golds_in_scc, enemies_in_scc, scc_area;

int dist_orig_board_reversible[HEIGHT][WIDTH][HEIGHT][WIDTH];
int dist_orig_board_with_dig[HEIGHT][WIDTH][HEIGHT][WIDTH];
int dist_curr_board_no_dig[HEIGHT][WIDTH][HEIGHT][WIDTH];
int dist_curr_board_with_dig[HEIGHT][WIDTH];

bool stuck[HEIGHT][WIDTH];
int reachable_gold[HEIGHT][WIDTH];
int nearest_gold_to_respawn;

bool SCC_leaf[HEIGHT*WIDTH];

UnionFind uf(HEIGHT*WIDTH);
vector<Gold> golds, golds2;
vector<Enemy> enemies,enemies2;

Game g;
vector<Position> nuggets;
StateEngine *se = NULL;

