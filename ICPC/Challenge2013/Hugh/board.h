#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__

#include <vector>

// Information about the game board (no information about the players/enemies)
class Board{
public:
  Item A[HEIGHT][WIDTH];
  int dist_no_fall[HEIGHT][WIDTH][HEIGHT][WIDTH];
  int dist_with_fall[HEIGHT][WIDTH][HEIGHT][WIDTH];

  void BFS_no_fall();
  void BFS_with_fall();

  vector<cmd> valid_moves(int row,int col,bool canDig) const;
  
  // Read in the board
  void read();
  
  // Initializes the board before anyone is on it
  //  -- Fills the "dist" arrays
  void read_and_init();
  
  // Determines if you can stand on a location.
  //  -- Only considers the map layout, not Enemies.
  bool canStand(int ROW,int COL) const;
};


void Board::read() {
  char c;
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      cin >> c;
      switch(c){
        case '.': A[i][j] = EMPTY; break;
        case 'H': A[i][j] = LADDER; break;
        case '=': A[i][j] = BRICK; break;
        case '*': A[i][j] = GOLD; break;
        case '-': A[i][j] = REMOVED_BRICK; break;
        default: assert(false);
      }
    }
}

bool Board::canStand(int ROW,int COL) const {
  if(!valid_indices(ROW,COL)) return false;
  
  switch(A[ROW][COL]){
    case BRICK: return false;
    case LADDER: return true;
    default:;
  }
  
  if(ROW == HEIGHT-1) return true;
  switch(A[ROW+1][COL]){
    case BRICK: return true;
    case LADDER: return true;
    default:;
  }
  return false;
}

vector<cmd> Board::valid_moves(int row,int col,bool canDig) const {
  vector<cmd> v(1,NONE);
  if(!canStand(row,col)) return vector<cmd>(1,DOWN); // Must be falling!
  if(A[row][col] == LADDER){
    if(row != 0 && A[row-1][col] != BRICK) v.push_back(UP);
    if(row != HEIGHT-1 && A[row+1][col] != BRICK) v.push_back(DOWN);
  } else {
    if(row != HEIGHT-1 && A[row+1][col] == LADDER) v.push_back(DOWN);
  }
  
  if(col > 0 && A[row][col-1] != BRICK) v.push_back(LEFT);
  if(col < WIDTH-1 && A[row][col+1] != BRICK) v.push_back(RIGHT);

  if(!canDig) return v;
  
  if(row == HEIGHT-1) return v; // No can dig
  
  if(col > 0)
    if(A[row][col-1] != BRICK && A[row][col-1] != LADDER)
      if(A[row+1][col-1] == BRICK)
	v.push_back(DIG_LEFT);
  
  if(col < WIDTH-1)
    if(A[row][col+1] != BRICK && A[row][col+1] != LADDER)
      if(A[row+1][col+1] == BRICK)
	v.push_back(DIG_RIGHT);
  
  return v;
}

void Board::BFS_no_fall(){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      for(int k=0;k<HEIGHT;k++)
        for(int l=0;l<WIDTH;l++)
          dist_no_fall[i][j][k][l] = -1;   // Not reachable
  
  // Calculate the distance without any falling. This is for "reversible path"
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      if(!canStand(i,j)) continue;
      
      queue<pii> q;
      dist_no_fall[i][j][i][j] = 0;
      q.push(make_pair(i,j));
      
      while(!q.empty()){
        pii t = q.front(); q.pop();
        int I = t.first, J = t.second;

	vector<cmd> v = valid_moves(I,J,false);
	for(size_t k=0;k<v.size();k++){
          int nI = I+di[v[k]], nJ = J+dj[v[k]];
          if(canStand(nI,nJ) && dist_no_fall[i][j][nI][nJ] == -1){
            dist_no_fall[i][j][nI][nJ] = dist_no_fall[i][j][I][J] + 1;
            q.push(make_pair(nI,nJ));
          }
        }
      }
    }
}

void Board::BFS_with_fall(){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      for(int k=0;k<HEIGHT;k++)
        for(int l=0;l<WIDTH;l++)
          dist_with_fall[i][j][k][l] = -1;   // Not reachable

  // Calculate the distance if we are allowed to fall. If we are "inside" of a
  //  brick then we will fall. Note that this means that this brick must have
  //  been broken.
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      queue<pii> q;
      dist_with_fall[i][j][i][j] = 0;
      q.push(make_pair(i,j));
      
      while(!q.empty()){
        pii t = q.front(); q.pop();
        int I = t.first, J = t.second;
	
	vector<cmd> v = valid_moves(I,J,false);
	for(size_t k=0;k<v.size();k++){
          int nI = I+di[v[k]], nJ = J+dj[v[k]];
          if(valid_indices(nI,nJ) && dist_with_fall[i][j][nI][nJ] == -1){
            dist_with_fall[i][j][nI][nJ] = dist_with_fall[i][j][I][J] + 1;
            q.push(make_pair(nI,nJ));
          }
        }
      }
    }
}

void Board::read_and_init() {
  read();
  
  BFS_no_fall();
  BFS_with_fall();
}

#endif
