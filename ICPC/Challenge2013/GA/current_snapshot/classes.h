#ifndef __UOFL_ICPC_CHALLENGE_CLASSES__
#define __UOFL_ICPC_CHALLENGE_CLASSES__

#include <iostream>
#include <vector>
#include <tr1/tuple>
#include <cassert>
#include <queue>
#include "globals.h"

using namespace std;
using namespace std::tr1;

////////////////////////////////////////////////////////////////////////////
// Union Find
////////////////////////////////////////////////////////////////////////////
struct UnionFind
{
  vector<int> uf;

  UnionFind(int n) : uf(n) {
    for (int i = 0; i < n; i++) uf[i] = i;
  }
  int find(int x) {
    return (uf[x] == x) ? x : (uf[x] = find(uf[x]));
  }
  bool merge(int x, int y) {
    int res1 = find(x), res2 = find(y);
    if (res1 == res2) return false;
    uf[res2] = res1;
    return true;
  }
};

extern UnionFind uf;

////////////////////////////////////////////////////////////////////////////
// Gold
////////////////////////////////////////////////////////////////////////////
struct Gold{
  int row,col,id;
};

extern vector<Gold> golds; //, golds2;

////////////////////////////////////////////////////////////////////////////
// Position
////////////////////////////////////////////////////////////////////////////
class Position {
public:
  Position(uchar r=0, uchar c=0): row(r), col(c) {}
  bool operator==(const Position &p) const { return (row == p.row) && (col == p.col); }
public:
  char row;
  char col;
};

ostream& operator<<(ostream &os, const Position &p);
istream& operator>>(istream &is, Position &p);

const Position NO_POS = Position(-1, -1);
extern vector<Position> nuggets;



////////////////////////////////////////////////////////////////////////////
// Move
////////////////////////////////////////////////////////////////////////////
class Move {
private:
  Move(char dr, char dc, char l) : dRow(dr), dCol(dc), label(l) {}
  
public:
  
  Position apply(const Position &p) const {
    return Position(p.row + dRow, p.col + dCol);
  }
  
  bool operator==(const Move &m) const {
    if (this == &m) return true;
    if (this->dRow != m.dRow)
      return false;
    if (this->dCol != m.dCol)
      return false;
    return true;
  }
  
  
public:
  char dRow;
  char dCol;
  char label;
  
  static const Move UP;
  static const Move DOWN;
  static const Move LEFT;
  static const Move RIGHT;
  static const Move NONE;
};

const Move& reverse(const Move &m);
const Move& getMove(cmd c);


////////////////////////////////////////////////////////////////////////////
// Board
////////////////////////////////////////////////////////////////////////////
typedef tuple<int,int,int,int,cmd> tiiiic; // Like the typedef name?
typedef priority_queue<tiiiic,vector<tiiiic>,greater<tiiiic> > PQ;

class Board {
private:
  int uf_index(int r, int c) const
  {
    return r * WIDTH + c;
  }

  /*
  void BFS_no_fall();
  void SS_BFS_no_fall(int r,int c,int dist[HEIGHT][WIDTH]);
  void SS_BFS_with_fall(int r,int c,int delay);
  void update_and_add(PQ& pq,vector<cmd> moves[HEIGHT][WIDTH],
		      int dist2[HEIGHT][WIDTH][11],
		      int nDist,int nI,int nJ,int nD,cmd nC);
  */
public:
  
  
  /* ============================================
     Storage
     
     The board will be stored as a 2D array of unsigned chars.  However, I've used a bit of a different mapping
     than what the game does:
     
     1 : empty
     2 : ladder
     3 : gold cell
     4-153:  removed gold cell, will resurface in val-3 turns
     154: brick
     155-179 : removed brick by our player, will resurface in val-154 turns
     180-204 : removed brick by our opponent, will resurface in val-179 turns
     205-229 : removed brick by both opponents, will resurface in val-204 turns
     
     Using this approach, we can store complete information regarding cells using single chars for each cell.
     
     ============================================= */
  
  Board() {}
  
  vector<cmd> valid_moves(int row,int col,bool canDig) const;
  vector<Move> valid_movesM(int row,int col,bool canDig) const;
  
  
  uchar operator[](const Position &p) const {
    return cells[(uchar)p.row][(uchar)p.col];
  }
  
  uchar& operator[](const Position &p) {
    return cells[(uchar)p.row][(uchar)p.col];
  }
  
  bool removedGold(const Position &p) const {
    return ((*this)[p] > 3) && ( (*this)[p] < 154 );
  }
  
  
  void removeBrick(const Position &p, bool isMe) {
    if (isMe) {
      if ((*this)[p] == 204)
	(*this)[p] = 229;
      else
	(*this)[p] = 179;
    } else {
      if ((*this)[p] == 179)
	(*this)[p] = 229;
      else
	(*this)[p] = 204;
    }
  }
  
  int brick_respawns_in(int r, int c) const
  {
    Position p(r,c);
    if (visible(p) != REMOVED_BRICK) return -1;
    if (cells[r][c] > 204) return cells[r][c] - 204;
    if (cells[r][c] > 179) return cells[r][c] - 179;
    if (cells[r][c] > 154) return cells[r][c] - 154;
#ifndef FINAL_VERSION
    assert(false);
#endif
    return -1;
  }
  
  int gold_respawns_in(int r, int c) const
  {
    if (cells[r][c] == 3) return -1;
    if (4 <= cells[r][c] && cells[r][c] <= 153) return cells[r][c]-3;
    return -1;
  }
  
  bool opp_dug_me(int r, int c) const
  {
    return cells[r][c] >= 180;
  }
  
  // returns what is visible.  Gold cells that have removed gold will return as empty.  Removed bricks will always
  // return as removed brick, regardless of who removed it.
  uchar visible(const Position &p) const {
    uchar c = (*this)[p];
    if (c == 1)
      return EMPTY;
    if (c == 2)
      return LADDER;
    if (c == 3)
      return GOLD;
    if (c < 154)
      return EMPTY;
    if (c == 154)
      return BRICK;
    return REMOVED_BRICK;
  }
  
  uchar visible(int r, int c) const;
  
  Board visibleBoard() const {
    Board b(*this);
    for (int row = 0; row < HEIGHT; row++) {
      for (int col = 0; col < WIDTH; col++) {
	Position p(row, col);
	b[p] = visible(p);
      }
    }
    return b;
  }
  
  // Initializes the board before anyone is on it
  //void init(int r,int c,int delay);
  
  /*
  // initializes the SCCs
  void initSCC();
  
  int scc_index(int r, int c) 
  {
    return uf.find(uf_index(r, c));
  }
  
  bool is_SCC_leaf(int r, int c)
  {
    return SCC_leaf[scc_index(r, c)];
  }
  */
  
  // Determines if you can stand on a location.
  //  -- Only considers the map layout, not Enemies.
  bool canStand(int r, int c) const;
  
public:  // not necessary, but I'm used to having my member variables in the bottom of my declaration beneath a modifier.
  
  uchar cells[HEIGHT][WIDTH];
};


ostream& operator<<(ostream &os, const Board &p);
istream& operator>>(istream &is, Board &p);

/////////////////////////////////////////////////////////////////////////
// Dude
/////////////////////////////////////////////////////////////////////////
class Dude {
public:
  
  Dude(const Position &p=Position(0,0)) : pos(p), death_count(0) {}
  
  bool isDead() const { return pos == Position(-1,-1); }
  
public:
  Position pos;
  int death_count; // death count is the number of remaining turns until alive again.
};

/////////////////////////////////////////////////////////////////////////
// Player
/////////////////////////////////////////////////////////////////////////
class Player : public Dude {
public:
  
  Player(const Position &p=Position(0,0)) :  Dude(p), score(0), mason_count(0) {}
  
public:
  int score;
  uchar mason_count;
};

istream& operator>>(istream &is, Player &p);
ostream& operator<<(ostream &os, const Player &p);

/////////////////////////////////////////////////////////////////////////
// Enemy
/////////////////////////////////////////////////////////////////////////

class Enemy : public Dude {
public:
  
  Enemy(const Position &p=Position(0,0)) : Dude(p), ip(0), master(-1), lastMove(0) { }
  
public:
  uchar ip; // instruction pointer
  string stack; // stack of moves
  char master; // 0 - me, 1 - me, -1 - you
  int lastMove;
  char respawn_row, respawn_col;
};

istream& operator>>(istream &is, Enemy &p);
ostream& operator<<(ostream &os, const Enemy &p);

extern vector<Enemy> enemies,enemies2;

/////////////////////////////////////////////////////////////////////////
// Game
/////////////////////////////////////////////////////////////////////////
class Game {
public:
  Game() : turns(0) { }
  
public:
  int turns;
  Board board;
  Position me, you;
  vector<Position> enemyPos;
  vector<string> enemyProgram;
};

istream& operator>>(istream &is, Game &g);
ostream& operator<<(ostream &os, const Game &g);
extern Game g;

/////////////////////////////////////////////////////////////////////////
// State
/////////////////////////////////////////////////////////////////////////
class State {
public:
  State() : turn(0) {}
  State(const Game &g);

  bool fallingDude(const Dude &p) const;
  bool fallingEnemy(const Enemy &p) const;
  bool legal(const Position &p) const;
  bool canNonFall(const Dude &p, const Move &move) const;

  void applyNonFall(const Dude &p, Dude &newp, const Move &move) const;
  void applyMoveDude(const Dude &p, Dude &newp, const cmd &command) const;

  void update(const Move *prevMove[HEIGHT][WIDTH],
	      Position prevPosition[HEIGHT][WIDTH],
	      queue< pair<Position, int> > &myqueue, const Position &position,
	      int step, const Position &prev, const Move &move) const;

  bool fallingPursuit(const Position &pos) const;

  pair<const Move *, int> getPursuitMove(const Enemy &enemy,
					 const Player &player,
					 const Move **moves, int limit) const;

  void applyMoveEnemy(const Enemy &enemy, Enemy &newEnemy,
		      string program, bool left, int turn) const;

  void killDude(Dude &d, uchar turns) const {
    d.pos = Position(-1, -1);
    d.death_count = turns;
  }

  void restoreDude(Dude &d, const Position &start) {
    d.pos = start;
  }

  bool canDestroyBrick(const Player &p, const Position &sideCell,
		       const Position &targetCell) const;

  void respawn(Dude &d, const Position &start) const {
    if (d.death_count > 0) {
      d.death_count--;
      if (d.death_count == 0) {
	d.pos = start;
      }
    }
  }

  void checkForCollision(Player &p) {
    if (p.isDead())
      return;
    for (unsigned int i = 0; i < enemies.size(); i++) {
      if (p.pos == enemies[i].pos) {
	killDude(p, 50);
	return;
      }
    }
  }

  State getNewState(const Game &g, cmd myCommand, cmd yourCommand) const {
    State newState(*this);
    return getNewState(g, myCommand, yourCommand, newState);
  }

  State& getNewState(const Game &g, cmd myCommand, cmd yourCommand,
		     State &newState) const;


public:
  int turn;
  Board board;
  bool visited[HEIGHT][WIDTH];
  Player me, you;
  vector<Enemy> enemies;
};

istream& readInState(istream &is, State &g, int enemyCount);
ostream& operator<<(ostream &os, const State &s);


/////////////////////////////////////////////////////////////////////////
// StateEngine
/////////////////////////////////////////////////////////////////////////

class StateEngine {
public:

  StateEngine(const Game &pg, bool kgoe=true)
    : g(pg), gameStatus(new State(g)), ourStatus(0), killGameOnError(kgoe) {}

  void next(istream &is, const string &myMove);

public:

  const Game g;
  State *gameStatus;
  State *ourStatus;
  bool killGameOnError;
};

extern StateEngine *se;

/////////////////////////////////////////////////////////////////////////
// Lookahead
/////////////////////////////////////////////////////////////////////////
class Lookahead {
public:

  static void getViableCommands( const State &status, const Player &p,
				 vector<cmd> &commands );

private:

  static double boundedLookaheadScore( const Game &g, const State &status,
				       int depth, int max_depth,
				       int turns_remaining, State *stack );

public:

  static int boundedLookahead( const Game &g, const State &status,
			       int maxDepth, int turns_remaining,
			       vector<cmd> &potentials );


  static double boundedLookaheadScoreABD( const Game &g, const State &status,
  					 int depth, int max_depth,
  					 int turns_remaining, State *stack );

  static int boundedLookaheadABD( const Game &g, const State &status,
  				 int maxDepth, int turns_remaining,
  				 int threshold, vector<cmd> &potentials );

  static double boundedLookaheadScoreNewDecay( const Game &g, const State &status,
					 int depth, int max_depth,
					 int turns_remaining, int oldScore, double currScore, double decay, State *stack );
};


/////////////////////////////////////////////////////////////////////////
// Utilities
/////////////////////////////////////////////////////////////////////////

// this is a sanity check.  It is a superficial check that compares to what
// we are receiving from the game engine.

bool equivalent(const Player &e1, const Player &e2);
bool equivalent(const Enemy &e1, const Enemy &e2);
bool equivalent(const State &s1, const State &s2, bool verbose=false);


#endif
