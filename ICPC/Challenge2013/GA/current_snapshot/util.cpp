#include "globals.h"
#include "classes.h"
#include "util.h"

void read_world() {
  cin >> g;
  
  se = new StateEngine(g, false);
  
  respawn_row = g.me.row;
  respawn_col = g.me.col;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      stuck[i][j] = true;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      vector<cmd> v = g.board.valid_moves(i,j,true);
      if(find(v.begin(),v.end(),DIG_LEFT)  != v.end()) stuck[i][j] = false;
      if(find(v.begin(),v.end(),DIG_RIGHT) != v.end()) stuck[i][j] = false;
      if(g.board.visible(i,j) == BRICK) stuck[i][j] = false;
      if(!g.board.canStand(i,j)) stuck[i][j] = false;
    }
  
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      BFS__(g.board,dist_orig_board_reversible[i][j],i,j);
      BFSDF(g.board,dist_orig_board_with_dig[i][j],0,i,j,false);
    }

  for (size_t i = 0; i < g.enemyPos.size(); i++) {
    int r = g.enemyPos[i].row, c = g.enemyPos[i].col;
    for(int k=0;k<HEIGHT;k++)
      for(int l=0;l<WIDTH;l++)
	if(dist_orig_board_reversible[r][c][k][l] < oo)
	  stuck[k][l] = false;
  }

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++) {
      if (!stuck[i][j]) continue;
      int dist[HEIGHT][WIDTH];
      BFS_F(g.board, dist, i, j);
      for(int k=0;k<HEIGHT && stuck[i][j];k++)
	for(int l=0;l<WIDTH && stuck[i][j];l++)
	  stuck[i][j] &= 
	    (stuck[k][l] || dist[k][l] == oo);
    }

  
  //  g.board.initSCC();

  alive_time = -1;

  Position p;
  for (int row = 0; row < HEIGHT; row++) {
    p.row = row;
    for (int col = 0; col < WIDTH; col++) {
      p.col = col;
      if (g.board.visible(p) == GOLD)
	nuggets.push_back(p);
    }
  }
  
  NUMBER_OF_TURNS = g.turns;
  NUMBER_OF_ENEMIES = g.enemyPos.size();
  TURNS_REMAINING = NUMBER_OF_TURNS;
  /*
  for(int i=0;i<NUMBER_OF_ENEMIES;i++){
    Position &p = g.enemyPos[i];
    enemies_in_scc[g.board.scc_index(p.row, p.col)]++;
  }
  
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      scc_area[g.board.scc_index(i,j)]++;
  */

  nearest_gold_to_respawn = oo;
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(g.board.visible(i,j) == GOLD){
	Gold G;
	G.row = i;
	G.col = j;
	G.id = golds.size();
	golds.push_back(G);
	if(dist_orig_board_with_dig[respawn_row][respawn_col][i][j] < nearest_gold_to_respawn)
	  nearest_gold_to_respawn = dist_orig_board_with_dig[respawn_row][respawn_col][i][j];
	//	golds_in_scc[g.board.scc_index(i, j)]++;
      }
  
  int stuck_ctr = 0;
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(!stuck[i][j] && g.board.visible(i,j) == GOLD)
	stuck_ctr++;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(!stuck[i][j])
	reachable_gold[i][j] = stuck_ctr;

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      if (stuck[i][j]) {
	reachable_gold[i][j] = 0; 
	int dist[HEIGHT][WIDTH];
	BFS_F(g.board, dist, i, j);
	for(size_t k=0;k<golds.size();k++)
	  if(dist[golds[k].row][golds[k].col] != oo)
	    reachable_gold[i][j]++;
      }
    }
  enemies.resize(NUMBER_OF_ENEMIES);
  enemies2.resize(NUMBER_OF_ENEMIES);
}

void read_turn(const string &move){
  //  string move = "NONE";
  se->next(cin, move);

 // End of game (we must have missed a turn...)
  if(se->gameStatus->turn == -1) return;

  TURNS_REMAINING = NUMBER_OF_TURNS - se->gameStatus->turn;
  
  Player &P1 = se->gameStatus->me;
  if(P1.isDead()) alive_time = -1;
  else if(alive_time == -1) alive_time = 0;
  else alive_time++;

  if(!P1.isDead()){
    for(int i=0;i<HEIGHT;i++)
      for(int j=0;j<WIDTH;j++)
	BFS_F(g.board,dist_curr_board_no_dig[i][j],i,j);
    BFSDF(g.board,dist_curr_board_with_dig,P1.pos.row,P1.pos.col,
	  P1.mason_count,true);
  }
  // The next line can be completely taken out if Darcy's thing works...
  // g.board.init(P1.pos.row,P1.pos.col,P1.mason_count);
  
  // **************************************************************
  //  Everything to do with gold
  // **************************************************************

  /*
  Board &B = (se->ourStatus) ? se->ourStatus->board : se->gameStatus->board;
  golds_in_scc.clear();
  for(size_t i=0;i<golds.size();i++){
    if(board(golds[i].row,golds[i].col) == GOLD){
      golds_in_scc[g.board.scc_index(golds[i].row, golds[i].col)]++;
    } else {
      if (B.gold_respawns_in(golds[i].row, golds[i].col) <= 2) {
	golds_in_scc[g.board.scc_index(golds[i].row, golds[i].col)]++;
      }
    }
  }
  for (map<int,int>::iterator it = golds_in_scc.begin(); it != golds_in_scc.end(); ++it) {
    cerr << "(" << it->first << ", " << it->second << ") ";
  }
  cerr << endl;
  */
  
  sort(golds.begin(),golds.end(),gold_dist);
  /*
  golds2 = golds;
  if (alive())
    sort(golds2.begin(),golds2.end(),gold_dist2);
  */

  // **************************************************************
  //  Everything to do with enemies
  // **************************************************************

  // Deal with enemies...
  for(int i=0;i<NUMBER_OF_ENEMIES;i++) {
    Enemy &E = se->gameStatus->enemies[i];
    enemies[i] = E;
    enemies[i].respawn_row = g.enemyPos[i].row;
    enemies[i].respawn_col = g.enemyPos[i].col;
    enemies2[i] = enemies[i];
  }
  sort(enemies.begin(),enemies.end(),enemy_dist_to_you);
  sort(enemies2.begin(),enemies2.end(),enemy_dist_to_them);
}

string output_command( cmd C ){
  // Add any error checking you need here.
  //  For example, should we ever go into a hole that is about to close?
  const string COMMAND_NAME[] =
  { "TOP","BOTTOM","LEFT","RIGHT","DIG_LEFT","DIG_RIGHT","NONE" };
  cout << COMMAND_NAME[C] << endl;
  return COMMAND_NAME[C];
}

string output_command2( cmd C ){
  // Add any error checking you need here.
  //  For example, should we ever go into a hole that is about to close?
  const string COMMAND_NAME[] =
  { "TOP","BOTTOM","LEFT","RIGHT","DIG_LEFT","DIG_RIGHT","NONE" };
  cerr << COMMAND_NAME[C] << " -- " << int(C) << endl;
  cout << COMMAND_NAME[C] << endl;
  return COMMAND_NAME[C];
}

bool valid_indices(int ROW,int COL){
  return (0 <= ROW && ROW < HEIGHT && 0 <= COL && COL < WIDTH);
}

Item board(int row,int col) {
  Board &board = se->gameStatus->board;
  return board.visible(row,col);
}

int distance(int r1,int c1,int r2,int c2) {
  if(!valid_indices(r1,c1) || !valid_indices(r2,c2)) return oo;

  Position &P1 = se->ourStatus->me.pos;
  if(r1 == P1.row && c1 == P1.col)
    return dist_curr_board_with_dig[r2][c2];
  return dist_curr_board_no_dig[r1][c1][r2][c2];
}

int distance_estimate(int r1,int c1,int r2,int c2) {
  if(!valid_indices(r1,c1) || !valid_indices(r2,c2)) return oo;

  Position &P1 = se->ourStatus->me.pos;
  if(r1 == P1.row && c1 == P1.col)
    return dist_curr_board_with_dig[r2][c2];
  return min(dist_orig_board_with_dig[r1][c1][r2][c2],dist_curr_board_no_dig[r1][c1][r2][c2]);
}

static vector<cmd> valid_moves(const Player &P)
{
  if (P.isDead()) return vector<cmd>(1,NONE);
  return se->gameStatus->board.valid_moves(P.pos.row, P.pos.col, 
					  P.mason_count == 0);
}

vector<cmd> valid_moves() {
  Player &P1 = se->gameStatus->me;
  return valid_moves(P1);
}

vector<cmd> valid_moves2() {
  Player &P2 = se->gameStatus->you;
  return valid_moves(P2);
}

bool can_hammer(){
  vector<cmd> v = valid_moves();
  for(size_t i=0;i<v.size();i++)
    if(v[i] == DIG_LEFT || v[i] == DIG_RIGHT)
      return true;
  return false;
}

bool alive() {
  Player &P1 = se->ourStatus->me;
  return !P1.isDead();
}

bool falling(){
  Position &P1 = se->ourStatus->me.pos;
  return alive() && !se->ourStatus->board.canStand(P1.row,P1.col);
}

void curr_loc(int& r,int & c) {
  Position &P1 = se->ourStatus->me.pos;
  r = P1.row; c = P1.col;
}

pair<cmd,int> suicide_move(){
  int m1 = oo,  m2 = oo;
  cmd c1 = NONE,c2 = NONE;

  vector<cmd> moves = valid_moves();

  Board &B = (se->ourStatus) ? se->ourStatus->board : g.board;
  
  // Method 1: Run straight to enemy
  Position &P1 = (se->ourStatus) ? se->ourStatus->me.pos :
    se->gameStatus->me.pos;
  if(!enemies2.empty()){
    vector<cmd> v = Goto(P1.row,P1.col,enemies2[0].pos.row,enemies2[0].pos.col);
    if(!v.empty()){
      // As you get close, they will walk towards you. If you are their master,
      //  then it will reduce it by 2. If the opponent is their master, then it
      //  will reduce it by 4.
      int fudge_factor = (enemies2[0].master == 1 ? 4 : 2);
      m1 = max(0,distance(P1.row,P1.col,enemies2[0].pos.row,enemies2[0].pos.col) - fudge_factor);
      c1 = v[0];
    }
  }
  
  // Method 2: Dig a hole and jump in
  //  Method 2(a): Am I already in a hole?
  if(board(P1.row,P1.col) == REMOVED_BRICK) {
    if (B.opp_dug_me(P1.row, P1.col)) {  // get out if we can
      for (size_t i = 0; i < moves.size(); i++) {
	if (moves[i] == LEFT && !B.opp_dug_me(P1.row, P1.col-1)) {
	  return make_pair(moves[i], B.brick_respawns_in(P1.row, P1.col-1));
	}
	if (moves[i] == RIGHT && !B.opp_dug_me(P1.row, P1.col+1)) {
	  return make_pair(moves[i], B.brick_respawns_in(P1.row, P1.col+1));
	}
      }
    } 
    return make_pair(NONE,B.brick_respawns_in(P1.row, P1.col));
  }
  
  //  Method 2(b): Is there a hole already dug nearby?
  for (int i = P1.col-1; i >= 0; --i) {
    // Is this even a hole?
    if (board(P1.row+1, i) != REMOVED_BRICK) continue;
    // Is there an enemy in that hole?
    bool en = false;
    for(size_t j=0;j<enemies.size();j++)
      if(enemies[j].pos.row == P1.row+1 && enemies[j].pos.col == i)
	en = true;
    if(en) continue;
    int dist = distance(P1.row,P1.col,P1.row,i);
    // Will this hole close up before we can get there?
    if (dist > B.brick_respawns_in(P1.row+1, i)) continue;
    // Could we get trapped in this hole?
    if (P1.row+2 < HEIGHT && board(P1.row+2,i) != BRICK &&
	dist != B.brick_respawns_in(P1.row+1,i)) continue;

    // If the opponent dug the hole, DON'T GO NEAR!!! In fact, stop the search,
    // because we would fall into the hole when we reached it.
    if (B.opp_dug_me(P1.row+1, i)) break;
    // Otherwise, we can consider this hole.
    int val = B.brick_respawns_in(P1.row+1, i) + dist;
    if(val < m2){
      c2 = LEFT;
      m2 = val;
    }
    break; // We can't walk past this hole, so stop searching.
  }
  for (int i = P1.col+1; i < WIDTH; ++i) {
    // Is this even a hole?
    if (board(P1.row+1, i) != REMOVED_BRICK) continue;
    // Is there an enemy in that hole?
    bool en = false;
    for(size_t j=0;j<enemies.size();j++)
      if(enemies[j].pos.row == P1.row+1 && enemies[j].pos.col == i)
	en = true;
    if(en) continue;
    int dist = distance(P1.row,P1.col,P1.row,i);
    // Will this hole close up before we can get there?
    if (dist > B.brick_respawns_in(P1.row+1, i)) continue;
    // Could we get trapped in this hole?
    if (P1.row+2 < HEIGHT && board(P1.row+2,i) != BRICK &&
	dist != B.brick_respawns_in(P1.row+1,i)) continue;

    // If the opponent dug the hole, DON'T GO NEAR!!! In fact, stop the search,
    // because we would fall into the hole when we reached it.
    if (B.opp_dug_me(P1.row+1, i)) break;
    // Otherwise, we can consider this hole.
    int val = B.brick_respawns_in(P1.row+1, i) + dist;
    if(val < m2){
      c2 = RIGHT;
      m2 = val;
    }
    break; // We can't walk past this hole, so stop searching.
  }
  
  /*
  if(board(P1.row+1,P1.col-1) == REMOVED_BRICK){
    int val = B.brick_respawns_in(P1.row+1, P1.col-1) + 2;
    if(val < m2){
      c2 = LEFT;
      m2 = val;
    }
  }
  if(board(P1.row+1,P1.col+1) == REMOVED_BRICK){
    int val = B.brick_respawns_in(P1.row+1, P1.col+1) + 2;
    if(val < m2){
      c2 = RIGHT;
      m2 = val;
    }
  }
  */
  
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

cmd getCommand(const string &x) {
  if (x == "NONE")
    return NONE;
  if (x == "LEFT")
    return LEFT;
  if (x == "RIGHT")
    return RIGHT;
  if (x == "BOTTOM")
    return DOWN;
  if (x == "TOP")
    return UP;
  if (x == "DIG_LEFT")
    return DIG_LEFT;
  if (x == "DIG_RIGHT")
    return DIG_RIGHT;
  return NONE;
}

bool any_enemies(){
  return enemies.size() && !enemies[0].isDead() &&
    se->ourStatus->board.visible(enemies[0].pos.row,enemies[0].pos.col) != REMOVED_BRICK;
}

int dist_to_enemy(const Enemy& E,bool youGoingToThem, int r, int c){
  Board &board = se->ourStatus->board;
  bool alive = !E.isDead() &&
    board.visible(E.pos.row,E.pos.col) != REMOVED_BRICK;
  int extra = 0;

  // I intentionally have this as 30, not 25. You can replace it with a more accurate value
  //   if applicable...
  if(!alive)
    extra = (board.visible(E.pos.row,E.pos.col) == REMOVED_BRICK ? 30
	     : E.death_count);
  
  int eRow = alive ? E.pos.row : E.respawn_row;
  int eCol = alive ? E.pos.col : E.respawn_col;

  if(youGoingToThem){
    return distance(r,c,eRow,eCol) + extra;
  } else {
    return distance(eRow,eCol,r,c) + extra;
  }
}

int dist_to_enemy(const Enemy& E,bool youGoingToThem)
{
  Position &pos = se->ourStatus->me.pos;
  return dist_to_enemy(E, youGoingToThem, pos.row, pos.col);
}

bool enemy_dist_to_you(const Enemy& e1, const Enemy& e2){
  return dist_to_enemy(e1,false) < dist_to_enemy(e2,false);
}

bool enemy_dist_to_them(const Enemy& e1,const Enemy& e2){
  return dist_to_enemy(e1,true) < dist_to_enemy(e2,true);
}

int dist_to_gold(const Gold& G){
  Position &pos = se->gameStatus->me.pos;
  Board &board = se->ourStatus->board;
  int d1 = distance(pos.row,pos.col,G.row,G.col);
  int d2 = (board.visible(G.row,G.col) == GOLD ? 0 :
	    board.gold_respawns_in(G.row, G.col));
  return max(d1,d2);    
}

int dist_to_goldP2(const Gold& G){
  Position &pos = se->gameStatus->you.pos;
  Board &board = se->ourStatus->board;
  int d1 = distance(pos.row,pos.col,G.row,G.col);
  int d2 = (board.visible(G.row,G.col) == GOLD ? 0 :
	    board.gold_respawns_in(G.row, G.col));
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

/*
bool gold_dist2(const Gold& G1,const Gold& G2){
  Position &P1 = se->gameStatus->me.pos;
  //  Position &P2 = se->ourStatus->you.pos;
  
  int player_scc = g.board.scc_index(P1.row, P1.col);
  //  int opp_scc = currBoard.scc_index(P2.row, P2.col);
  int G1_scc = g.board.scc_index(G1.row, G1.col);
  int G2_scc = g.board.scc_index(G2.row, G2.col);

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
*/

vector<cmd> Goto(int r1,int c1,int r2,int c2){
  int dist = distance(r1,c1,r2,c2);
  if(dist == oo) return vector<cmd>(1,NONE);
  if(dist == 0)  return vector<cmd>(1,NONE);

  return nextmove_dont_use_me_use_Goto_instead[r1][c1][r2][c2];
}

void end_up(cmd C, int &r, int &c)
{
  curr_loc(r, c);

  Board &board = se->ourStatus->board;
  vector<cmd> list = board.valid_moves(r, c, false);
  if (find(list.begin(), list.end(), C) == list.end()) {
    C = NONE;
  }

  if (C == NONE) return;
  r += di[C];
  c += dj[C];

  while (!board.canStand(r, c)) r++;
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
    if (board(enemies[i].pos.row, enemies[i].pos.col) == REMOVED_BRICK)
      continue;
    double d = (r-enemies[i].pos.row)*(r-enemies[i].pos.row) +
      (c-enemies[i].pos.col)*(c-enemies[i].pos.col);
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


int level_of_suicide(){
  const int FIRST_GOLD_THRESHOLD = 65;
  const int SECOND_GOLD_THRESHOLD = 75;
  const int FIRST_ENEMY_THRESHOLD = 4;
  const int SECOND_ENEMY_THRESHOLD = 8;
  const int RESPAWN_THRESHOLD = 25;

  int gold1 = (golds.size() >= 1 ? dist_to_gold(golds[0]) : oo);
  int gold2 = (golds.size() >= 2 ? dist_to_gold(golds[1]) : oo);
  int enemy1 = (enemies2.size() >= 1 ? dist_to_enemy(enemies2[0],true) : oo);
  int enemy2 = (enemies2.size() >= 2 ? dist_to_enemy(enemies2[1],true) : oo);
  
  // Case 1: Not 25 turns within respawning, or if number of turns left is
  //         too small
  if(alive_time <= RESPAWN_THRESHOLD) return 0;
  if(TURNS_REMAINING <= PLAYER_RESPAWN_TIME) return 0;
  if(gold1 < nearest_gold_to_respawn + 50 + suicide_move().second) return 0;
  
  // Case 2: If the two nearest gold pieces are too far away, do it now.
  if(gold1 > FIRST_GOLD_THRESHOLD && gold2 > SECOND_GOLD_THRESHOLD){
    // Case 2(a): If the next two enemies are close, I will kill one and
    //  be killed by the other (hopefully)
    if(enemy1 <= FIRST_ENEMY_THRESHOLD && enemy2 <= SECOND_ENEMY_THRESHOLD)
      return 1;
    // Case 2(b): They are too far away, kill yourself now.
    return 3;
  }

  // Case 3: Two pieces of gold are mid-ranged.
  if(gold1 > FIRST_GOLD_THRESHOLD && gold2 <= SECOND_GOLD_THRESHOLD){
    // Case 3(a): If the next two enemies are close, I will kill one and
    //  be killed by the other (hopefully)
    if(enemy1 <= FIRST_ENEMY_THRESHOLD && enemy2 <= SECOND_ENEMY_THRESHOLD)
      return 1;
    // Case 3(b): They are too far away, kill yourself now.
    return 2;
  }

  // Case 4: One close piece of gold, one far.
  if(gold1 <= FIRST_GOLD_THRESHOLD && gold2 > SECOND_GOLD_THRESHOLD){
    // Case 4(a): Are you close-ish?
    if(gold1 <= 7) return 1;
    // Case 4(b): Not close.
    return 0;
  }

  // Case 5: Both close.
  if(gold1 <= FIRST_GOLD_THRESHOLD && gold2 <= SECOND_GOLD_THRESHOLD){
    // Case 5(a): Are you close-ish?
    if(gold2 <= 7) return 1;
    // Case 5(b): Not close.
    return 0;
  }
  return 0; // I shouldn't get here... Could do an assert(false)
}

// Dig/Fall
void BFS__(Board& B,int dist[HEIGHT][WIDTH],int r,int c){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      dist[i][j] = oo;

  if(!B.canStand(r,c)) return; // No falling allowed!
  
  queue<pii> q;
    
  dist[r][c] = 0;
  q.push(make_pair(r,c));
  
  while(!q.empty()){
    pii t = q.front(); q.pop();
    int I = t.first, J = t.second;
      
    vector<cmd> v = B.valid_moves(I,J,false);
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(B.canStand(nI,nJ) && dist[nI][nJ] == oo){
	vector<cmd> v2 = B.valid_moves(nI,nJ,false);
	if(find(v2.begin(),v2.end(),opp[v[k]]) == v2.end()) continue;
	dist[nI][nJ] = dist[I][J] + 1;
	q.push(make_pair(nI,nJ));
      }
    }
  }
}

void BFS_F(Board& B,int dist[HEIGHT][WIDTH],int r,int c){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      dist[i][j] = oo;
  
  queue<pii> q;
  
  dist[r][c] = 0;
  q.push(make_pair(r,c));
  
  while(!q.empty()){
    pii t = q.front(); q.pop();
    int I = t.first, J = t.second;
      
    vector<cmd> v = B.valid_moves(I,J,false);
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(dist[nI][nJ] == oo){
	dist[nI][nJ] = dist[I][J] + 1;
	q.push(make_pair(nI,nJ));
      }
    }
  }
}

// Used by BFS_with_fall
void update_and_add(PQ& pq,vector<cmd> moves[HEIGHT][WIDTH],
		    int dist[HEIGHT][WIDTH],
		    int dist2[HEIGHT][WIDTH][11],int nDist,
		    int nI,int nJ,int nD,cmd nC){
  if(dist2[nI][nJ][nD] < nDist) return; // We already have better!
  
  if(dist[nI][nJ] == nDist) // Add this move to our list!
    if(find(moves[nI][nJ].begin(),moves[nI][nJ].end(),nC) == moves[nI][nJ].end())
      moves[nI][nJ].push_back(nC);
  
  if(dist[nI][nJ] > nDist){
    dist[nI][nJ] = nDist;
    moves[nI][nJ].clear();       // Remove old results
    moves[nI][nJ].push_back(nC); // Put me in!
  }
  
  if(dist2[nI][nJ][nD] > nDist){
    dist2[nI][nJ][nD] = nDist;
    pq.push(make_tuple(nDist,nI,nJ,nD,nC));
  }
}

// Moreover, this will break bricks and attempt to commit suicide
void BFSDF(Board& B,int dist[HEIGHT][WIDTH],int r,int c,int delay,bool update_nextmove){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      dist[i][j] = oo;

  PQ pq;
  
  vector<cmd> moves[HEIGHT][WIDTH];
  int dist2[HEIGHT][WIDTH][11];
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      for(int k=0;k<=10;k++) dist2[i][j][k] = oo;
      dist[i][j] = oo;
    }
    
  dist2[r][c][delay] = dist[r][c] = 0;
  moves[r][c].push_back(NONE);
  pq.push(make_tuple(0,r,c,delay,NONE));
    
  pair<cmd,int> suicide = suicide_move();
  suicide.second += PLAYER_RESPAWN_TIME + 5;
  
  update_and_add(pq,moves,dist,dist2,suicide.second,
		 respawn_row,respawn_col,0,suicide.first);
  
  int I,J,D,myDist;
  cmd C;
  for(bool first=true;!pq.empty();first=false){
    tie(myDist,I,J,D,C) = pq.top(); pq.pop();
      
    if(myDist > dist2[I][J][D] && !first) continue;
      
    vector<cmd> v = B.valid_moves(I,J,(D == 0));
      
    // Deal with one very special case:
    //  If there is a hole here, but it may not "actually" be here...
    //  This implementation only works for when you will be WALKING
    //   over the hole, not when you jump from up high into it...
      
    for(int CMD=LEFT;CMD<=RIGHT;CMD++)
      if(find(v.begin(),v.end(),CMD) != v.end())
	if(B.visible(I+1,J+dj[CMD]) == REMOVED_BRICK){
	  cmd nC = (first ? NONE : C);
	  int nI = I;
	  int nJ = J+2*dj[CMD];
	  if(!valid_indices(I,J+2*dj[CMD])) continue;
	  int respawn = B.brick_respawns_in(I+1, J+dj[CMD]);
#ifndef FINAL_VERSION
	  if(respawn < 0) cerr << respawn << endl;
	  assert(respawn >= 0);
#endif
	  int nD = max(0,D - respawn);
	  int nDist = respawn + 2; // NONEs + 2 CMDs
	  update_and_add(pq,moves,dist,dist2,nDist,nI,nJ,nD,nC);
	}
      
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(!valid_indices(nI,nJ)) continue;
	
      // The next line is 8 because:
      //  first you dig (at turn t), then move left (at turn t+1), then down (at turn t+2)
      int nD = max(0,(v[k] == DIG_LEFT || v[k] == DIG_RIGHT ? 8 : D-1));
      int nDist = myDist + dijkstra_dist[v[k]];
      cmd nC = (first ? v[k] : C);
	
      update_and_add(pq,moves,dist,dist2,nDist,nI,nJ,nD,nC);
    }
  }

  if(update_nextmove)
    for(int i=0;i<HEIGHT;i++)
      for(int j=0;j<WIDTH;j++){
	sort(moves[i][j].begin(),moves[i][j].end());
	nextmove_dont_use_me_use_Goto_instead[r][c][i][j] = moves[i][j];
      }
}
