#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

// Size of the playing field.
const int FIELD_ROW_COUNT = 16;
const int FIELD_COLUMN_COUNT = 25;

// How close an enemy can be before we try to move away or trap it.
const int THREAT_THRESHOLD = 5;

// Constants for the various directions the player can move.
enum MoveDir { LEFT, TOP, RIGHT, BOTTOM };

// Names for the different directions.
string moveName[] = { "LEFT", "TOP", "RIGHT", "BOTTOM" };
  
// Row and column offsets for the move directions.
int dRow[] = { 0, -1, 0, 1 };
int dCol[] = { -1, 0, 1, 0 };
  
/** Representation for a player or enemy position. */
struct Position {
  int r, c;

  Position() : r( 0 ), c( 0 ) {
  }
};

/** Simple interface for reading positions from standard input. */
istream &operator>>( istream &input, Position &pos ) {
  input >> pos.r >> pos.c;
  return input;
}

/** Return true if a player will fall through this map character. */
bool isAir( char ch ) {
  return !( ch == '=' || ch == 'H' );
}

int main( int argc, char* argv[] ) {
  // Initial positions and current positions for each runner.
  Position initRunners[ 2 ], runners[ 2 ];

  // Current score in the game.
  int scores[ 2 ];

  // How long until each runner can dig.
  int digDelay[ 2 ];

  // How long will the game be?
  int turns;
  cin >> turns;

  // Read a picture of the map.
  string world[ FIELD_ROW_COUNT ];
  for (int i = 0; i < FIELD_ROW_COUNT; i++)
    cin >> world[ i ];

  // Time whn we last visited this space, or -1.
  vector< vector< int > > visitTime( FIELD_ROW_COUNT, vector< int >( FIELD_COLUMN_COUNT, -1 ) );

  // Read initial positions of the two runners.
  for(int i = 0; i < 2; i++)
    cin >> initRunners[i];

  // Read initial positions of all the enemies.
  int enemyCount;
  cin >> enemyCount;

  // Respawn spot for each enemy.
  vector< Position > initEnemies( enemyCount );

  // What player each enemy is afraid of (or -1), not used in this player.
  vector< int > master( enemyCount, -1 );

  // Circuit the enemy will make while patrolling.  Not useed in this player.
  vector< string > program( enemyCount );

  for(int i = 0; i < enemyCount; i++)
    cin >> initEnemies[i] >> program[ i ];

  // Current enemy positions.
  vector< Position > enemies = initEnemies;

  // Last known move direction for each enemy.
  vector< int > enemyMoveDir( enemyCount );

  // For each turn in the game.
  for ( int turnNumber = 0; turnNumber < turns; turnNumber++ ) {
    // Read the turn number (should be [ 0, turs ) unless we're late with a move.
    int turn;
    cin >> turn;

    // Read the current world description, since gold may be collected
    // and the floor may be destroyed.
    for (int i = 0; i < FIELD_ROW_COUNT; i++)
      cin >> world[ i ];
    
    // Read the positions of each player's runners, current scores and
    // how long until each can dig.
    for(int i = 0; i < 2; i++)
      cin >> runners[i] >> scores[ i ] >> digDelay[ i ];

    // For convenience, remember my position in some local variables.
    int myRow = runners[ 0 ].r;
    int myCol = runners[ 0 ].c;

    // Remember previous enemy positions, and read latest ones.
    vector< Position > prevEnemies = enemies;
    for ( int i = 0; i < enemyCount; i++ ) 
      cin >> enemies[ i ] >> master[ i ];

    // Tentative move.  See if we can do better.
    string move = "NONE";
    if ( myRow >= 0 && myCol >= 0 ) {

      // Remember when I visited this cell.
      visitTime[ myRow ][ myCol ] = turn;

      // Is there an enemy heading toward us from the left or right?
      bool approachingLeft = false, approachingRight = false;

      // Can we move left, up, right or down?
      vector< bool > canMove( 4, false );

      // Is there a nearby enemy left, up, right or down?
      vector< bool > threat( 4, false );

      for ( int i = 0; i < enemyCount; i++ ) {
        // If this enemy has moved, remember its move direction.
        if ( enemies[ i ].r > prevEnemies[ i ].r )
          enemyMoveDir[ i ] = BOTTOM;
        else if ( enemies[ i ].r < prevEnemies[ i ].r )
          enemyMoveDir[ i ] = TOP;
        else if ( enemies[ i ].c > prevEnemies[ i ].c )
          enemyMoveDir[ i ] = RIGHT;
        else if ( enemies[ i ].c < prevEnemies[ i ].c )
          enemyMoveDir[ i ] = LEFT;

        // See if there are enemies nearby
        if ( enemies[ i ].r == myRow && enemies[ i ].c < myCol &&
             myCol - enemies[ i ].c < THREAT_THRESHOLD ) {
          threat[ LEFT ] = true;

          if ( myCol - enemies[ i ].c > 1 && enemyMoveDir[ i ] == RIGHT )
            approachingLeft = true;
        }

        if ( enemies[ i ].r == myRow && enemies[ i ].c > myCol &&
             enemies[ i ].c - myCol < THREAT_THRESHOLD ) {
          threat[ RIGHT ] = true;

          if ( enemies[ i ].c - myCol > 1 && enemyMoveDir[ i ] == LEFT )
            approachingRight = true;
        }

        if ( enemies[ i ].c == myCol && enemies[ i ].r < myRow &&
             myRow - enemies[ i ].r < THREAT_THRESHOLD )
          threat[ TOP ] = true;

        if ( enemies[ i ].c == myCol && enemies[ i ].r > myRow &&
             enemies[ i ].r - myRow < THREAT_THRESHOLD )
          threat[ BOTTOM ] = true;
      }

      // Can we dig left or right?
      bool canDig = ( digDelay[ 0 ] == 0 );
      bool canDigLeft = false, canDigRight = false;

      // Look around the runner and see what it can do.
      if ( myCol > 0 && world[ myRow ][ myCol - 1 ] != '=' ) {
        canMove[ LEFT ] = true;

        if ( myRow + 1 < FIELD_ROW_COUNT ) {
          if ( world[ myRow ][ myCol - 1 ] != 'H' && world[ myRow + 1 ][ myCol - 1 ] == '=' &&
               ( world[ myRow + 1 ][ myCol ] == '=' || world[ myRow + 1 ][ myCol ] == 'H' ) )
            canDigLeft = canDig;
        }
      }
      
      if ( myCol + 1 < FIELD_COLUMN_COUNT && world[ myRow ][ myCol + 1 ] != '=' ) {
        canMove[ RIGHT ] = true;

        if ( myRow + 1 < FIELD_ROW_COUNT ) {
          if ( world[ myRow ][ myCol + 1 ] != 'H' && world[ myRow + 1 ][ myCol + 1 ] == '=' &&
               ( world[ myRow + 1 ][ myCol ] == '=' || world[ myRow + 1 ][ myCol ] == 'H' ) )
            canDigRight = canDig;
        }
      }
      
      if ( myRow > 0 && world[ myRow ][ myCol ] == 'H' && world[ myRow ][ myCol ] != '=' ) {
        canMove[ TOP ] = true;
      }
      
      if ( myRow + 1 < FIELD_ROW_COUNT && world[ myRow + 1 ][ myCol ] != '=' ) {
        canMove[ BOTTOM ] = true;
      }

      // First, try to trap an enemy if we can.
      if ( canDigLeft && approachingLeft ) {
        move = "DIG_LEFT";
      } else if ( canDigRight && approachingRight ) {
        move = "DIG_RIGHT";
      } else {
        vector< pair< int, int > > moves;
  
        // First, try to select from threat-free moves.
        for ( int i = 0; i < 4; i++ )
          if ( canMove[ i ] && !threat[ i ] )
            moves.push_back( make_pair( visitTime[ myRow + dRow[ i ] ][ myCol + dCol[ i ] ], i ) );
      
        // If there are none, chose from among any other moves.
        if ( moves.size() == 0 ) {
          for ( int i = 0; i < 4; i++ )
            if ( canMove[ i ] )
              moves.push_back( make_pair( visitTime[ myRow + dRow[ i ] ][ myCol + dCol[ i ] ], i ) );
        }

        if ( moves.size() ) {
          // Randomly choose among equally good moves.
          random_shuffle( moves.begin(), moves.end() );
          stable_sort( moves.begin(), moves.end() );

          move = moveName[ moves.front().second ];
        }
      }
    }

    // Send the chosen move to the game.
    cout << move << endl;
  }
}
