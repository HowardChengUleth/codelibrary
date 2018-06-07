#ifndef CHRIS_H
#define CHRIS_H

#include "util.h"
#include "heuristics.h"
#include <cmath>

const double PI = acos(-1.0);


vd Randomness(const vector<bool>& moves) {
  vd results(7);
  for (int i = 0; i < 7; ++i) results[i] = ((double)(rand() % 4096)/2048.0 - 1);
  return results;
}


bool WillGetStuck(int r, int c) {
  if (!valid_indices(r, c)) return false;
  
  for (int i = r; i < HEIGHT; ++i) {
    bool filled = board(i, c) == BRICK;
    for (int j = 0; j < enemies.size(); ++j)
      if (enemies[j].row == i && enemies[j].col == c)
	filled |= board(i, c) == REMOVED_BRICK;
    if (P2.row == i && P2.col == c)
      filled |= board(i, c) == REMOVED_BRICK;
    if (filled)
      return false;
    
    if ((board(i, c) == REMOVED_BRICK || board(i, c) == EMPTY) &&
	(i+1 >= HEIGHT || board(i+1, c) == BRICK) &&
	(!valid_indices(i, c-1) || board(i, c-1) == BRICK) &&
	(!valid_indices(i, c+1) || board(i, c+1) == BRICK))
      return true;
  }
  return false;
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
  if (WillGetStuck(r+1, c-1)) results[LEFT] = -1.0;
  if (WillGetStuck(r+1, c+1)) results[RIGHT] = -1.0;
  if (WillGetStuck(r+1, c))   results[DOWN] = -1.0;
  
  return results;
}


vd RandomClosestGold(const vector<bool>& moves) {
  static int chosen_r, chosen_c;
  static int ngolds = 0;
  static bool init = true;
  static int r,c, closest;
  curr_loc(r,c);

  if (init) {
    closest = 401; // Too large...
    int best_r=0,best_c=0;
    for(int i=0;i<HEIGHT;i++)
      for(int j=0;j<WIDTH;j++)
	if(board(i,j) == GOLD){
	  ++ngolds;
	  if(currBoard.dist_with_fall[r][c][i][j] == -1) continue;
	  if(closest > currBoard.dist_with_fall[r][c][i][j]){
	    closest = currBoard.dist_with_fall[r][c][i][j];
	    best_r = i;
	    best_c = j;
	  }
	}
    chosen_r = best_r, chosen_c = best_c;
    init = false;
  }
  else if (CURRENT_TURN % 100 == 0) {
    for (int i = 0; i < HEIGHT; ++i)
      for (int j = 0; j < WIDTH; ++j)
	if (board(i,j) == GOLD && rand() % ngolds == 0) {
	  chosen_r = i;
	  chosen_c = j;
	  closest = currBoard.dist_with_fall[r][c][i][j];
	  goto done;
	}
    done:
    ;
  }
  
  vd ans(7,0);
  // Prefer not to dig.
  ans[DIG_LEFT] = ans[DIG_RIGHT] = -0.5;

  double val = 1,dist = closest;
  
  if(dist > NUMBER_OF_TURNS - CURRENT_TURN) return vd(7,0); // Don't care
  
  if(dist <= 2) val = 1;
  else if(dist <= 5) val = 0.7;
  else if(dist <= 7) val = 0.5;
  else if(dist <= 10) val = 0.2;
  else if(dist <= 15) val = 0.1;
  else val = 0.05;

  for(int k=0;k<7;k++){
    if(!moves[k]) continue;
    int nI = r+di[k], nJ = c+dj[k];
    if(!valid_indices(nI,nJ)) continue;
    if(closest - 1 == currBoard.dist_with_fall[nI][nJ][chosen_r][chosen_c])
      ans[k] = val;
  }
  
  return ans;
}


#endif
