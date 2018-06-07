#ifndef __UOFL_ICPC_CHALLENGE_HEURISTICS__
#define __UOFL_ICPC_CHALLENGE_HEURISTICS__

#include "util.h"
#include<cassert>
#include<cmath>
// *****************************************************************************
// *****************************************************************************
//  All heuristics must:
//    - Take one argument -- vector<bool> moves.
//        - (moves[i] == true) iff (i is a valid move)
//    - Return a vector of doubles of size 7.
//        - It doesn't matter what you put for invalid moves
//    - Every element must be between -1 and 1 (inclusive)
//        - +1 means you REALLY want this move to happen
//        - -1 means you REALLY don't want this
//        - 0 is indifferent
//    - Don't forget about the time remaining
//
//  Assumptions:
//    - You are alive.
//    - You are not falling.
//
//  The order:
//   enum cmd { UP , DOWN, LEFT, RIGHT, DIG_LEFT, DIG_RIGHT, NONE };
//               0     1     2     3       4         5         6

//
// *****************************************************************************
// *****************************************************************************

#include "ClosestGoldLookahead.hh"

vd closest_gold(const vector<bool>& moves){
  vd ans(7,0);
  if(golds.empty()) return ans;

  int dist = dist_to_gold(golds[0]);
  double val = max(0.2, 1 - (dist/5)*0.07);
    
  if(dist > NUMBER_OF_TURNS - CURRENT_TURN) return ans; // Don't care
  
  ans[DIG_LEFT] = ans[DIG_RIGHT] = -0.5;

  Position &P1 = se->ourStatus->me.pos;
  for(size_t i=0;i<golds.size();i++){
    int disti = dist_to_gold(golds[i]);
    if(disti != dist) break;
    vector<cmd> v = Goto(P1.row,P1.col,golds[i].row,golds[i].col);
    for(size_t j=0;j<v.size();j++) {
      // Are we falling?
      int r, c; end_up(v[j], r, c);
      if (distance(P1.row,P1.col,r,c) > 1) {
	// Is the gold we want in this column, and if so will it have
	// respawned by the time we hit it?
	if (golds[i].col == c &&
	    golds[i].row >= P1.row && golds[i].row <= r &&
	    distance(P1.row,P1.col,golds[i].row,golds[i].col) < disti)
	{
	  ans[NONE] = val;
	}
	else
	  ans[v[j]] = val;
      }
      else
	ans[v[j]] = val;
    }
  }

  if(ans[NONE] > 0) ans[DIG_LEFT] = ans[DIG_RIGHT] = 0;
  
  return ans;
}

vd closest_3gold_TSP(const vector<bool>& moves){
  if(golds.size() <= 1) return closest_gold(moves);
  
  vd ans(7,0);
  Board &board = se->ourStatus->board;

  int best = 3*oo,best_i = 0;
  
  if(golds.size() == 2){
    for(int i=0;i<2;i++){
      int d1 = dist_to_gold(golds[i]) +
	dist_orig_board_with_dig[golds[i].row][golds[i].col][golds[1-i].row][golds[1-i].col];
      int d2 = (board.visible(golds[1-i].row,golds[1-i].col) == GOLD ? 0 :
		board.gold_respawns_in(golds[1-i].row,golds[1-i].col));
      int dist = max(d1,d2);
      if(dist < best){
	best_i = i;
	best = dist;
      }
    }
    double val = max(0.2, 1 - (best/10)*0.07);
    Position &P1 = se->ourStatus->me.pos;
    vector<cmd> v = Goto(P1.row,P1.col,golds[best_i].row,golds[best_i].col);
    for(size_t i=0;i<v.size();i++)
      ans[v[i]] = val;
    return ans;
  }
  
  for(size_t i=0;i<golds.size();i++){
    int dist_i = dist_to_gold(golds[i]);
    if(dist_i + 2 >= best) continue;
    for(size_t j=0;j<golds.size();j++){
      if(i == j) continue;
      int dist_j = dist_i + 
	dist_orig_board_with_dig[golds[i].row][golds[i].col][golds[j].row][golds[j].col];
      int dist_j2 = (board.visible(golds[j].row,golds[j].col) == GOLD ? 0 :
		     board.gold_respawns_in(golds[j].row,golds[j].col));
      dist_j = max(dist_j,dist_j2);
      if(dist_j + 1 >= best) continue;
      for(size_t k=0;k<golds.size();k++){
	if(i == k || j == k) continue;
	int dist_k = dist_j +
	  dist_orig_board_with_dig[golds[j].row][golds[j].col][golds[k].row][golds[k].col];
	int dist_k2 = (board.visible(golds[k].row,golds[k].col) == GOLD ? 0 :
		       board.gold_respawns_in(golds[k].row,golds[k].col));
	dist_k = max(dist_k,dist_k2);
	if(dist_k < best){
	  best_i = i;
	  best = dist_k;
	}
      }
    }
  }
  
  double val = max(0.2, 1 - (best/15)*0.07);
  Position &P1 = se->ourStatus->me.pos;
  vector<cmd> v = Goto(P1.row,P1.col,golds[best_i].row,golds[best_i].col);
  for(size_t i=0;i<v.size();i++)
    ans[v[i]] = val;
  return ans;
}

/*
vd closest_gold_in_scc(const vector<bool>& moves){
  vd ans(7,0);
  if(golds2.empty()) return ans;

  int dist = dist_to_gold(golds2[0]);
  double val = max(0.2, 1 - (dist/5)*0.07);
    
  if(dist > NUMBER_OF_TURNS - CURRENT_TURN) return ans; // Don't care
  
  ans[DIG_LEFT] = ans[DIG_RIGHT] = -0.5;
  
  Position &P1 = se->ourStatus->me.pos;
  for(size_t i=0;i<golds2.size();i++){
    if(dist_to_gold(golds2[i]) != dist_to_gold(golds2[0]))
      break;
    vector<cmd> v = Goto(P1.row,P1.col,golds2[i].row,golds2[i].col);
    for(size_t j=0;j<v.size();j++)
      ans[v[j]] = val;
  }

  if(ans[NONE] > 0) ans[DIG_LEFT] = ans[DIG_RIGHT] = 0;
  
  return ans;
}
*/

vd dig_asap(const vector<bool>& moves){
  if(!moves[DIG_LEFT] && !moves[DIG_RIGHT]) return vd(7,0);
  
  vd ans(7,-1);
  if(moves[DIG_LEFT]) ans[DIG_LEFT] = 1;
  if(moves[DIG_RIGHT]) ans[DIG_RIGHT] = 1;
  return ans;
}

vd kill_enemy(const vector<bool>& moves){
   int r,c; curr_loc(r,c);
   vd ans(7,0);
   if(!moves[DIG_LEFT] && !moves[DIG_RIGHT]) return ans;
   if (!any_enemies()) return ans;

   size_t badguy = 0;
   for(; badguy<enemies.size(); badguy++)
      if(enemies[badguy].pos.row == r) break;

   if(badguy == enemies.size()) return ans;
   if (distance(r,c,enemies[badguy].pos.row,enemies[badguy].pos.col) >= TURNS_REMAINING)
     return ans;
   
   if(distance(r,c,enemies[badguy].pos.row,enemies[badguy].pos.col) > 5) 
     return ans;
   if(enemies[badguy].pos.col > c){
     int rightdist = enemies[badguy].pos.col - c;
     switch(rightdist){
       case 1: case 2: ans[DIG_RIGHT] = 1; break;
       case 3: ans[DIG_RIGHT] = ans[RIGHT] = 0.5; break;
       case 4: case 5: ans[RIGHT] = 1;
     }
   } else {
     int leftdist = c - enemies[badguy].pos.col;
     switch(leftdist){
       case 1: case 2: ans[DIG_LEFT] = 1; break;
       case 3: ans[DIG_LEFT] = ans[LEFT] = 0.5; break;
       case 4: case 5: ans[LEFT] = 1;
     }
   }
   
   return ans;
}

vd avoid_enemy(const vector<bool>& moves){
  int r,c; curr_loc(r,c);
  vd ans(7,0);
  if(!any_enemies()) return ans;
  if (TURNS_REMAINING == 1) return ans;

  if (moves[LEFT] && (r < HEIGHT-1 && board(r+1, c-1) == REMOVED_BRICK) && c-2 >= 0) {
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      if (enemies[i].pos.row == r && enemies[i].pos.col == c-2) {
	ans[LEFT] = -1;
	return ans;
      }
    }
  }
  if (moves[RIGHT] && (r < HEIGHT-1 && board(r+1, c+1) == REMOVED_BRICK) && c+2 < WIDTH) {
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      if (enemies[i].pos.row == r && enemies[i].pos.col == c+2) {
	ans[RIGHT] = -1;
	return ans;
      }
    }
  }
  
  int dist = dist_to_enemy(enemies2[0], false, r, c);
  if (dist >= TURNS_REMAINING) return ans;
  if(dist < 4){
    for (int d = 0; d < 4; d++) {
      if (!moves[d]) continue;
      int r2 = r + di[d], c2 = c+dj[d];
      int d2 = dist_to_enemy(enemies2[0], false, r2, c2);
      if (d2 > dist) {
	ans[d] = 0.5;
      } else if (d2 < dist) {
	ans[d] = -1;
	if (can_hammer()) {
	  if (d == LEFT) ans[DIG_LEFT] = 1;
	  if (d == RIGHT) ans[DIG_RIGHT] = 1;
	}
      }
    }
  }
  
  return ans;
}

vd visit_unvisited(const vector<bool>& moves){
  int r,c;
  curr_loc(r,c);
  
  double closest = oo; // Too large...
  int best_r=0,best_c=0;
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(!se->ourStatus->visited[i][j]){
	int Dist = distance(r,c,i,j);
	if(Dist == oo) continue;
	if(closest > Dist){
	  closest = Dist;
	  best_r = i;
	  best_c = j;
	}
      }
  
  vd ans(7,0);
  
  if(closest > NUMBER_OF_TURNS - CURRENT_TURN) return vd(7,0); // Don't care
  double val = 1.0/ceil((closest+.07)/5);
  
  vector<cmd> v = Goto(r,c,best_r,best_c);
  for(size_t i=0;i<v.size();i++)
    ans[v[i]] = val;
  
  return ans;
}

vd avoid_falling_death(const vector<bool>& moves) {
  int r,c;
  curr_loc(r,c);

  vd ans(7, 0);
  for (int k = 0; k < 7; k++) {
    if (!moves[k]) continue;
    int r2, c2;
    end_up(delta[k], r2, c2);

    // try not to fall into a place close to an enemy
    int mindist=401;
    for(size_t i=0; i<enemies.size(); i++)
      mindist = min(mindist, 
		    distance(enemies[i].pos.row,enemies[i].pos.col,r2,c2));
    mindist *= 2;      // enemy moves at half the speed
    int time = distance(r,c,r2,c2);
    if (time > TURNS_REMAINING) continue;
    
    int diff = (time > mindist) ? time - mindist : mindist - time;
    if (diff <= 4) {
       ans[k] = -1 + 0.1*diff;
    }

    // try not to fall into hole, but it's not as bad if it's far away
    if (board(r2, c2) == REMOVED_BRICK && time <= 25) {
      ans[k] = min(ans[k], -1 + 0.02*time);
    }
  }
  return ans;
}

vd kill_yourself(const vector<bool>& moves){
  if(level_of_suicide() < 2) return vd(7,0);
  
  vd ans(7,-1);
  ans[suicide_move().first] = 1;
  return ans;
}


vd wait_for_close(const vector<bool>& moves){
   return vd(7,0);
}

vd chase_opp(const vector<bool> &moves){
   vd ans(7,0);
   int r,c;
   curr_loc(r,c);

   Position &P2 = se->ourStatus->you.pos;
   vector<cmd> v = Goto(r,c,P2.row,P2.col);
   for(size_t i=0;i<v.size();i++)
     ans[v[i]] = 1;
   return ans;
}

vd kill_opp_dist0(const vector<bool> &moves) {
  vd ans(7, 0);
  int r,c;
  curr_loc(r,c);
  Position &P2 = se->ourStatus->you.pos;
  if (r==P2.row && c == P2.col && can_hammer()) {
     ans = vd(7,-1);
     if(golds.size() > 0){
	ans[DIG_LEFT] = ans[DIG_RIGHT] = 1; 
	vector<cmd> v = Goto(r,c,golds[0].row,golds[0].col);
	for(size_t i=0;i<v.size();i++){
	   if(v[i]==LEFT){ ans[DIG_LEFT] = 1; ans[DIG_RIGHT] = -1;}
	   if(v[i]==RIGHT){ ans[DIG_LEFT] = -1; ans[DIG_RIGHT] = 1;}
	   
	}
     } else
	ans[DIG_LEFT] = ans[DIG_RIGHT] = 1; 
     ans[NONE] = 0;
  }
  return ans;
}

vd kill_opp_dist2(const vector<bool> &moves) {
   vd ans(7, -1);
   int r,c;
   curr_loc(r,c);
//     for(int i=0;i<500000000;i++){curr_loc(r,c);}
      
   Position &P2 = se->ourStatus->you.pos;
   if (distance(r, c, P2.row, P2.col) <= 2 &&
       distance(r, c, P2.row, P2.col) != 0 &&
       can_hammer() && r == P2.row) {
      vector<cmd> v = Goto(r, c, P2.row, P2.col);
      for (size_t i = 0; i < v.size(); i++) {
	 if (v[i] == LEFT) {
	    ans[DIG_LEFT] = 1;
	 } else if (v[i] == RIGHT) {
	    ans[DIG_RIGHT] = 1;
	 }
     }
   }
   return ans;
}

vd gold_trap(const vector<bool> &moves){
  vd ans(7, 0);
  if (golds.size() == 0) return ans;
  int r,c;
  curr_loc(r,c);
  Position &P2 = se->ourStatus->you.pos;
  if (distance(r, c, P2.row, P2.col) <= 5 && dist_to_gold(golds[0]) == 1) {
     (r-golds[0].row == 1 ? ans[DIG_RIGHT] = 1 : ans[DIG_LEFT] = 1);
  }
  return ans;
}
/*
vd dig_under_falling_P2(const vector<bool> &moves){
   vd ans(7,0);
   int r,c;
   curr_loc(r,c);
   

}
*/ 
vd IHateDeathAndZombies(const vector<bool>& moves) {
  vd results(7, 0.0); // Initially we don't care
  int r, c; curr_loc(r, c);

  // Try to get away from the nearest 2 zombies. The strength of this
  // response will be inversly proportional to the distance.
  double dist = 1e18;
  int idx = -1;
  for (int i = 0; i < (int)enemies.size(); ++i) {
    // If this enemy is actually stuck in a hole, then ignore him.
    if (board(enemies[i].pos.row, enemies[i].pos.col) == REMOVED_BRICK)
      continue;
    double d = (r-enemies[i].pos.row)*(r-enemies[i].pos.row)*4.0 +
      (c-enemies[i].pos.col)*(c-enemies[i].pos.col);
    if (d < dist) { idx = i; dist = d; }
  }
  if (idx < 0) return results;
  if (distance(r,c,enemies[idx].pos.row,enemies[idx].pos.col) >= TURNS_REMAINING)
    return results;
  
  dist = sqrt(dist);
  double dir = atan2(r-enemies[idx].pos.row, c-enemies[idx].pos.col);
  
  if (dir > PI/4 && dir < 3*PI/4) {
    results[DOWN] = 1.0 / dist;
    results[UP] = -1.0 / dist;
  }
  else if (dir < -PI/4 && dir > -3*PI/4) {
    results[UP] = 1.0 / dist;
    results[DOWN] = -1.0 / dist;
  }
  else if (dir <= PI/4 && dir >= -PI/4) {
    if (CouldFallOnZombie(r+1, c+1))
      results[NONE] = 1.0 - 1.0 / dist;
    else {
      results[LEFT] = -1.0 / dist;
      results[DIG_LEFT] = 0.5 / dist;
      results[RIGHT] = 1.0 / dist;
    }
  }
  else {
    if (CouldFallOnZombie(r+1, c-1))
      results[NONE] = 1.0 - 1.0 / dist;
    else {
      results[RIGHT] = -1.0 / dist;
      results[DIG_RIGHT] = 0.5 / dist;
      results[LEFT] = 1.0 / dist;
    }
  }
  
  // Will moving left or right get us stuck in a hole? It's better to
  // face a zombie than get ourselves stuck ...
  if (WillGetStuck(LEFT)) results[LEFT] = -1.0;
  if (WillGetStuck(RIGHT)) results[RIGHT] = -1.0;
  if (WillGetStuck(DOWN))   results[DOWN] = -1.0;
  
  return results;
}

vd randomness(const vector<bool>& moves) {
  vd results(7);
  for (int i = 0; i < 7; ++i) results[i] = ((double)(rand() % 4096)/2048.0 - 1);
  return results;
}

vd never_run_into_opp_hole(const vector<bool>& moves){
  vd ans(7,0); // Indifferent initally...
  
  int r,c;
  for(int i=0;i<7;i++){
    end_up(cmd(i),r,c);
    if(board(r,c) == REMOVED_BRICK && se->ourStatus->board.opp_dug_me(r,c)){
       if(r==HEIGHT-1)
	  ans[i] = -1; // PLEASE DON'T GO!
       else if(board(r+1,c) == BRICK)
	  ans[i] = -1; // PLEASE DON'T GO!
    }
  }

  // If there is a way for us to die in an opponent's hole, strongly
  //  encourage everything else!
  for(int i=0;i<7;i++)
    if(ans[i] > -0.05)
      ans[i] = 1;
  
  return ans;
}

vd no_walk_into_filled_removed_brick(const vector<bool>& moves){
  vd ans(7,0);
  
  Position &P1 = se->ourStatus->me.pos;
  if(moves[LEFT] && board(P1.row,P1.col-1) == REMOVED_BRICK){
    for(size_t i=0;i<enemies.size();i++){
      if((enemies[i].pos.row == P1.row   && enemies[i].pos.col == P1.col-1) ||
	 (enemies[i].pos.row == P1.row-1 && enemies[i].pos.col == P1.col-1))
	ans[LEFT] = -1;
    }
  }

  if(moves[RIGHT] && board(P1.row,P1.col+1) == REMOVED_BRICK){
    for(size_t i=0;i<enemies.size();i++){
      if((enemies[i].pos.row == P1.row   && enemies[i].pos.col == P1.col+1) ||
	 (enemies[i].pos.row == P1.row-1 && enemies[i].pos.col == P1.col+1))
	ans[RIGHT] = -1;
    }
  }
  
  return ans;
}

vd kevins_lookahead(const vector<bool> &moves)
{
  vector<cmd> potentials;
  
  int score = Lookahead::boundedLookahead(g, *(se->ourStatus), 5,
					  TURNS_REMAINING, potentials);
  vd ans(7, 0);
  if (score <= 5) return ans;
//  cerr << "Score = " << score << endl;
  for (size_t i = 0; i < potentials.size(); i++) {
    ans[potentials[i]] = 1;
  }

  // done:
  return ans;
}

vd kill_opp_respawn(const vector<bool> &moves)
{
  // NOT YET IMPLEMENTED
  vd ans(7,0);
  return ans;
}

cmd lastcmd=NONE;
vd no_reverse(const vector<bool> &moves){
   vd ans(7,1);
   ans[opp[lastcmd]] = -1;
   ans[NONE] = 1;
   return ans;
}

vd avoid_stuck(const vector<bool>& moves) {
  int r,c;
  curr_loc(r,c);

  vd ans(7, 0);
  Position &P1 = se->ourStatus->me.pos;
  for (size_t i = 0; i < golds.size(); i++) {
    int r = golds[i].row, c = golds[i].col;
    if (!stuck[r][c]) {
      vector<cmd> v = Goto(P1.row, P1.col, r, c);
      double dist = dist_to_gold(golds[i]);
      double val = max(0.2, 1 - dist/5*0.07);
      for(size_t j=0;j<v.size();j++) {
	// Are we falling?
	int r, c; end_up(v[j], r, c);
	if (distance(P1.row,P1.col,r,c) > 1) {
	  // Is the gold we want in this column, and if so will it have
	  // respawned by the time we hit it?
	  if (golds[i].col == c &&
	      golds[i].row >= P1.row && golds[i].row <= r &&
	      distance(P1.row,P1.col,golds[i].row,golds[i].col) < dist)
	    {
	      ans[NONE] = val;
	    }
	  else
	    ans[v[j]] = val;
	}
	else
	  ans[v[j]] = val;
      }
      break;
    }
  }
  for (int k = 0; k < 7; k++) {
    if (!moves[k]) continue;
    int r2, c2;
    end_up(delta[k], r2, c2);
    if (stuck[r2][c2]) {
      ans[k] = -1;
    }
  }
  return ans;
}

/*
vd dont_go_into_leaf_scc(const vector<bool> &moves){
  vd ans(7,0);

  for (int i = 0; i < 7; i++) {
    if (!moves[i]) continue;
    int r, c;
    end_up(cmd(i), r, c);

    if (!g.board.is_SCC_leaf(r, c)) continue;
    ans[i] = -1 + 0.05 * golds_in_scc[g.board.scc_index(r, c)];
    ans[i] = min(ans[i], 1.0);
  }
  return ans;
}
*/

#endif
