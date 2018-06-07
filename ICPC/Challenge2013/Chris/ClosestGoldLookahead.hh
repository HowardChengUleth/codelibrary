#ifndef CLOSEST_GOLD_LOOKAHEAD_HH
#define CLOSEST_GOLD_LOOKAHEAD_HH


#include "util.h"
#include <utility>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;


#define N_LOOKAHEAD_STEPS 20

#define MINIMIZE(var,n) (var)=min((var),(n))
#define MAXIMIZE(var,n) (var)=max((var),(n))
  

class EnemyLookahead {
public:

  /**
     WIDTH x HEIGHT dimensions!
     With the current turn being 0, this stores the eariest turn an emeny could
     possibly get to position (x,y), or oo if an enemy can't get there in
     N_LOOKAHEAD_STEPS.
  */
  int **lookahead;

  /**
     Computes the enemy lookahead table. Call once per turn.

     The resulting table (should) never over-estimate the time it takes
     an enemy to reach a cell. It hopefully won't under-estimate by much
     either. This doesn't consider holes unless an enemy is initially
     caught in one. This is sufficient because enemies can't fall through
     holes.
  */
  void computeEnemies() {
    init();
    
    lookahead = lookahead_[0];
    for (int i = 1; i <= N_LOOKAHEAD_STEPS; ++i) {
      int **last = lookahead_[!(i&1)];
      lookahead = lookahead_[i&1];
      for (int x = 0; x < WIDTH; ++x)
	for (int y = 0; y < HEIGHT; ++y) {
	  // Is an enemy already here?
	  if (last[x][y] != oo) {
	    lookahead[x][y] = last[x][y]; continue;
	  }
	  else lookahead[x][y] = oo;
	  // Figure out how an enemy could get here (if possible).
	  if (!origBoard.canStand(y,x)) continue;
	  // Could an enemy possibly get here ...
	  // from above? (Note that they can fall faster than they can
	  // walk, so we do this every turn)
	  if (valid_indices(y-1,x) && last[x][y-1] != oo)
	    MINIMIZE(lookahead[x][y], last[x][y-1] + 1);
	  // from below?
	  if (valid_indices(y+1,x) && last[x][y+1] < i-1 &&
	      origBoard.itemAt(y+1,x) == LADDER)
	    MINIMIZE(lookahead[x][y], last[x][y+1] + 2);
	  // from the left?
	  if (valid_indices(y,x-1) && last[x-1][y] < i-1 &&
	      origBoard.canStand(y,x-1))
	    MINIMIZE(lookahead[x][y], last[x-1][y] + 2);
	  // from the right?
	  if (valid_indices(y,x+1) && last[x+1][y] < i-1 &&
	      origBoard.canStand(y,x+1))
	    MINIMIZE(lookahead[x][y], last[x+1][y] + 2);
	}
    }
    // We biased all our times by -1 in the beginning (in case last time
    // was the turn the enemy moved), so if we still have -1's in the
    // table, make them 0.
    for (int x = 0; x < WIDTH; ++x)
      for (int y = 0; y < HEIGHT; ++y)
	MAXIMIZE(lookahead[x][y], 0);
    
#ifdef CGL_DEBUG
    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x)
	cerr << setw(4) << lookahead[x][y] << " ";
      cerr << endl;
    }
    cerr << endl;
#endif
  }

  
  EnemyLookahead() {
    lookahead = NULL;
    lookahead_[0] = new int*[WIDTH];
    lookahead_[1] = new int*[WIDTH];
    for (int i = 0; i < WIDTH; ++i) {
      lookahead_[0][i] = new int[HEIGHT];
      lookahead_[1][i] = new int[HEIGHT];
    }
  }

  
  ~EnemyLookahead() {
    for (int i = 0; i < WIDTH; ++i) {
      delete[] lookahead_[0][i];
      delete[] lookahead_[1][i];
    }
    delete[] lookahead_[0];
    delete[] lookahead_[1];
  }

  
private:
  int **lookahead_[2];
  
  void init() {
    lookahead = NULL;
    for (int i = 0; i < WIDTH; ++i)
      fill_n(lookahead_[0][i], HEIGHT, oo);
    for (int i = 0; i < (int)enemies.size(); ++i)
      // Find where the enemy is located. If it's stuck, ignore it.
      if (valid_indices(enemies[i].row, enemies[i].col) &&
	  board(enemies[i].row, enemies[i].col) != REMOVED_BRICK)
	lookahead_[0][enemies[i].col][enemies[i].row] = -1;
  }
};


/**
   Returns a vector of all (gold) points that can be reached from
   (x,y). We take respawn times of the gold into consideration.
*/
vector<pii> RoutesAvailable(int x, int y) {
  vector<pii> res;
  for (int i = 0; i < (int)golds.size(); ++i) {
    pii dest = make_pair(golds[i].col, golds[i].row);
    int dist = distance(y,x,golds[i].row,golds[i].col);
    // Could we reach the destination by the time the gold respawns?
    if (dist != oo && dist > 0 && dist >= gold_respawns_in[golds[i].id])
      res.push_back(dest);
  }
  return res;
}


/**
   Class implementing the heuristic. See below for the method to be used
   in uleth.cpp.
*/
class CGL {
public:
  CGL() : curDest(make_pair(-1,-1)) { }
  
  /**
     Computes a weighting for each available move. It strongly encourages
     movement towards it's target gold, and away from death.
  */
  vd computeMoves(const vector<bool>& moves) {
    el.computeEnemies();
    vd res(7,-2); // We rescale to [-1,1] later.
    int x, y; curr_loc(y, x);

    vector<pii> destinations = RoutesAvailable(x,y);
    vd weights(destinations.size(), 0);
    vector<cmd> goodmoves(destinations.size(), NONE);
    // If we have no destination, then there's nothing we can do.
    if (!destinations.size()) return vd(7,0);
    
    int best = 0;
    int bestdist = oo;
    pii bestdest = make_pair(-1,-1);
    int curDestIdx = -1;
    
    // Iterate through all the gold that we can get to from this position,
    // and chose the best option.
    for (int i = 0; i < (int)destinations.size(); ++i) {
      pii dest = destinations[i];
      if (dest == curDest) curDestIdx = i;
      vector<cmd> nextMoves = Goto(y,x,dest.second,dest.first);
      
      // Try the moves that we can make.
      for (int j = 0; j < (int)nextMoves.size(); ++j) {
	cmd c = nextMoves[j];
	if (!moves[c]) continue;	
	double w = pathWeight(x+dj[c], y+di[c], dest);
	if (weights[i] < w) {
	  weights[i] = w; goodmoves[i] = c;
	}
	// Let this move be a possible move, although not the recommended
	// one (we deal with that later).
	MAXIMIZE(res[c], w / 2.0);
      }
      
      // Is this destination better than our current best?
      if (weights[i] > weights[best] ||
	  (weights[i] == weights[best] &&
	   distance(y,x,dest.second,dest.first) < bestdist))
      {
	best = i;
	bestdist = distance(y,x,dest.second,dest.first);
	bestdest = dest;
      }
    }

    // The weights we computed above are in [0,0.5]. Rescale them to
    // [-0.5,0.5]. Any -2's should become 0's (we used them to make the
    // MAXIMIZE code work).
    for (int i = 0; i < 7; ++i)
      if (res[i] < -1) res[i] = 0;
      else res[i] = (res[i]*2)-1; // rescale
    
    // We use a "sticky" path selection - use a path different from our
    // present one only if it is significantly better or we can no longer
    // reach the old one. This avoids having our player change
    // destinations frequently.
    if (curDestIdx < 0 || weights[curDestIdx] <= weights[best] * 0.75) {
      curDestIdx = best;
      curDest = bestdest;
    }
    // Recommend our path. We scale it to be in [-0.5, 1] (so that if it's
    // very bad, other heuristics can take over).
    res[goodmoves[curDestIdx]] = weights[curDestIdx] * 1.5 - 0.5;
    return res;
  }


private:

  EnemyLookahead el;
  pii curDest;
  
  double pathWeight(int x, int y, pii dest) {
    double weight = 1.0;
    for (int i = 1; i <= N_LOOKAHEAD_STEPS; ++i) {
      // Are we likely to hit an enemy? Note that the chances of this
      // occurring go down a lot the further ahead we look, because of
      // inaccuracies with our enemy prediction.
      if (el.lookahead[x][y] <= i)
	weight *= (double)(i-1) / i;
      // Are we going to fall into a hole?
      if (valid_indices(y+1,x) && board(y+1,x) == REMOVED_BRICK) {
	// Is there an enemy here that we can walk over?
	bool blocked = false;
	for (int j = 0; j < (int)enemies.size(); ++j)
	  if (enemies[j].row == y+1 && enemies[j].col == x) {
	    blocked = true; break;
	  }
	if (!blocked) {
	  int ey;
	  for (ey = y+1; ey < HEIGHT; ++ey) {
	    if (board(ey,x) == BRICK) break;
	  }
	  bool noLeft = x == 0 || board(ey-1,x-1) == BRICK;
	  bool noRight = x == WIDTH-1 || board(ey-1,x+1) == BRICK;
	  if ((ey == HEIGHT || board(ey,x) == BRICK) && noLeft && noRight) {
	    weight *= 0; // Stuck! Dislike!!!
	  }
	  else weight *= 0.75; // Not bad, but not recommended.
	}
      }
      // If we have somewhere to go, then go there.
      vector<cmd> moves = Goto(y,x,dest.second,dest.first);
      // Get rid of moves we don't want. We avoid digging because we
      // don't move anywhere.
      while (moves.size() &&
	     (moves.back() == NONE ||
	      moves.back() == DIG_LEFT ||
	      moves.back() == DIG_RIGHT))
	moves.pop_back();
      if (moves.size()) {
	// Just use the first move. This will probably prefer climbing.
	x += dj[moves[0]]; y += di[moves[0]];
      }
      // Otherwise, stop walking this way.
      else break;
    }
    return weight;
  }
};

CGL cgl_;


// **** Heuristic method ****
// Use like any other heuristic.


vd ClosestGoldLookahead(const vector<bool>& validMoves) {
  return cgl_.computeMoves(validMoves);
}


#undef MINIMIZE
#undef MAXIMIZE
#undef N_LOOKAHEAD_STEPS

#endif
