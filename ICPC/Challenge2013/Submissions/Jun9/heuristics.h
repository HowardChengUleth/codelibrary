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
  
  for(size_t i=0;i<golds.size();i++){
    if(dist_to_gold(golds[i]) != dist_to_gold(golds[0]))
      break;
    vector<cmd> v = Goto(P1.row,P1.col,golds[i].row,golds[i].col);
    for(size_t j=0;j<v.size();j++)
      ans[v[j]] = val;
  }

  if(ans[NONE] > 0) ans[DIG_LEFT] = ans[DIG_RIGHT] = 0;
  
  return ans;
}

vd closest_gold_in_scc(const vector<bool>& moves){
  vd ans(7,0);
  if(golds2.empty()) return ans;

  int dist = dist_to_gold(golds2[0]);
  double val = max(0.2, 1 - (dist/5)*0.07);
    
  if(dist > NUMBER_OF_TURNS - CURRENT_TURN) return ans; // Don't care
  
  ans[DIG_LEFT] = ans[DIG_RIGHT] = -0.5;
  
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
   
   size_t badguy = 0;
   for(; badguy<enemies.size(); badguy++)
      if(enemies[badguy].row == r) break;

   if(badguy == enemies.size()) return ans;
   if(distance(r,c,enemies[badguy].row,enemies[badguy].col) > 5) return ans;
   if(enemies[badguy].col > c){
      int rightdist = enemies[badguy].col - c;
     if(moves[DIG_RIGHT]){
       ans[DIG_RIGHT] = (1.0/ceil(((rightdist)+1)/4.0));
       if(rightdist > 2) ans[RIGHT] = 1;
     }
   } else {
     int leftdist = c - enemies[badguy].col;
     if(moves[DIG_LEFT]){
      ans[DIG_LEFT] = (1.0/ceil(((leftdist)+1)/4.0));
      if(leftdist > 2) ans[LEFT] = 1;
     }
   }
   
   return ans;
}

vd avoid_enemy(const vector<bool>& moves){
  int r,c; curr_loc(r,c);
  vd ans(7,0);
  if(!any_enemies()) return ans;

  if (moves[LEFT] && (r < HEIGHT-1 && board(r+1, c-1) == REMOVED_BRICK) && c-2 >= 0) {
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      if (enemies[i].row == r && enemies[i].col == c-2) {
	ans[LEFT] = -1;
	return ans;
      }
    }
  }
  if (moves[RIGHT] && (r < HEIGHT-1 && board(r+1, c+1) == REMOVED_BRICK) && c+2 < WIDTH) {
    for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
      if (enemies[i].row == r && enemies[i].col == c+2) {
	ans[RIGHT] = -1;
	return ans;
      }
    }
  }
  
  int dist = distance(r,c,enemies2[0].row,enemies2[0].col);
  if(dist < 4){
    ans[UP] = ans[DOWN] = ans[LEFT] = ans[RIGHT] = 0.5;  // run away
    vector<cmd> v = Goto(r,c,enemies2[0].row,enemies2[0].col);
    for(size_t i=0;i<v.size();i++)
      if (dist <= 2) {
	ans[v[i]] = -1;
      }
      else if (!can_hammer())  {
	ans[v[i]] = -1 + 0.25 * dist;
      } else {
	if (v[i] == LEFT) {
	  ans[DIG_LEFT] = 1;
	} else if (v[i] == RIGHT) {
	  ans[DIG_RIGHT] = 1;
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
      if(!visited_[i][j]){
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
  double val = 1.0/ceil(closest/5);
  
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
      mindist = min(mindist, distance(enemies[i].row,enemies[i].col,r2,c2));
    int time = distance(r,c,r2,c2);
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
   int distgold;
   int gold_threshold=65;
   int enemy_threshold=20;
   vd ans(7,0);
   int r,c; curr_loc(r,c);
   if(!golds.empty())
      distgold = dist_to_gold(golds[0]);
   else
      distgold = gold_threshold;
   int disten;
   if(!enemies.empty())
      disten = dist_to_enemy(enemies2[0],true);
   else
      disten = enemy_threshold;
   if(distgold >= gold_threshold && disten >= enemy_threshold){
     ans = vd(7,-1);
     ans[suicide_move().first] = 1;
   }
   return ans;
}


vd wait_for_close(const vector<bool>& moves){
   return vd(7,0);
}

vd chase_opp(const vector<bool> &moves){
   vd ans(7,0);
   int r,c;
   curr_loc(r,c);
   vector<cmd> v = Goto(r,c,P2.row,P2.col);
   for(size_t i=0;i<v.size();i++)
     ans[v[i]] = 1;
   return ans;
}

vd kill_opp(const vector<bool> &moves) {
  vd ans(7, 0);
  int r,c;
  curr_loc(r,c);
  if (distance(r, c, P2.row, P2.col) <= 3) {
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
  int r,c;
  curr_loc(r,c);
  if (distance(r, c, P2.row, P2.col) <= 5 && dist_to_gold(golds[0]) == 1) {
     (r-golds[0].row == 1 ? ans[DIG_RIGHT] = 1 : ans[DIG_LEFT] = 1);
  }
  return ans;
}

vd IHateDeathAndZombies(const vector<bool>& moves) {
  vd results(7, 0.0); // Initially we don't care
  int r, c; curr_loc(r, c);

  // Try to get away from the nearest 2 zombies. The strength of this
  // response will be inversly proportional to the distance.
  double dist = 1e18;
  int idx = 0;
  for (int i = 0; i < (int)enemies.size(); ++i) {
    // If this enemy is actually stuck in a hole, then ignore him.
    if (board(enemies[i].row, enemies[i].col) == REMOVED_BRICK)
      continue;
    double d = (r-enemies[i].row)*(r-enemies[i].row)*4.0 +
      (c-enemies[i].col)*(c-enemies[i].col);
    if (d < dist) { idx = i; dist = d; }
  }
  dist = sqrt(dist);
  double dir = atan2(r-enemies[idx].row, c-enemies[idx].col);
  //cerr << dir << " " << dist << endl;
  //if (dist > 6) ;
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

vd dont_walk_into_hole_horizontally(const vector<bool> &moves){
  vd ans(7, 0);
  int r,c;
  curr_loc(r,c);

  if (moves[LEFT] && board(r, c-1) == REMOVED_BRICK) {
    ans[LEFT] = -1.0;
  }
  if (moves[RIGHT] && board(r, c+1) == REMOVED_BRICK) {
    ans[RIGHT] = -1.0;
  }

  return ans;
}

#endif
