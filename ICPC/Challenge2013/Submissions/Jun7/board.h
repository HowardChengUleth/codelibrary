#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__

#include <vector>

bool visited_[HEIGHT][WIDTH];

// Information about the game board (no information about the players/enemies)
class Board{
public:
  Item A[HEIGHT][WIDTH];
  int dist_no_fall[HEIGHT][WIDTH][HEIGHT][WIDTH];
  int dist_with_fall[HEIGHT][WIDTH][HEIGHT][WIDTH];

  void BFS_no_fall();
  void BFS_with_fall(int delay);

  void SS_BFS_no_fall(int i,int j,int dist[HEIGHT][WIDTH]); // Single-source
  void SS_BFS_with_fall(int i,int j,int delay,int dist[HEIGHT][WIDTH]); // Single-source
  
  vector<cmd> valid_moves(int row,int col,bool canDig) const;
  
  // Read in the board
  void read();
  
  // Initializes the board before anyone is on it
  //  -- Fills the "dist" arrays
  void init(int);
  
  // Determines if you can stand on a location.
  //  -- Only considers the map layout, not Enemies.
  bool canStand(int ROW,int COL) const;
};

class Gold{
  public:
  int row,col,id;
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
      default:
	ofstream of("about_to_die.txt");
	for (int ii = 0; ii < HEIGHT; ii++) {
	  for (int jj = 0; jj < WIDTH; jj++) {
	    of << A[ii][jj];
	  }
	  of << endl;
	}
	of << "i, j = " << i << ", " << j << endl;
	of << "read '" << c << "'" << endl;
	of << "int value = " << (int) c << endl;
	of << "cin.good()? " << cin.good() << endl;
	assert(false);
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
    case REMOVED_BRICK:
      for(size_t i=0;i<enemies.size();i++)
	if(enemies[i].row == ROW+1 && enemies[i].col == COL)
	  return true;
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

  if (A[row+1][col] != BRICK && A[row+1][col] != LADDER) return v;
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
          dist_no_fall[i][j][k][l] = oo;   // Not reachable
  
  // Calculate the distance without any falling. This is for "reversible path"
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      if(canStand(i,j))
	SS_BFS_no_fall(i,j,dist_no_fall[i][j]);
  
  for (int i = 0; i < HEIGHT; i++) 
    for (int j = 0; j < WIDTH; j++)
      if (canStand(i,j)) 
	for (int k = 0; k < HEIGHT; k++) 
	  for (int l = 0; l < WIDTH; l++)
	    if (canStand(k,l)) {
	      nextmove[i][j][k][l].clear();
	      if (k == i && j == l) {
		nextmove[i][j][k][l].push_back(NONE); continue;
	      }
	      for (int dir = 0; dir < 4; dir++) {
		int ii = i + di[dir], jj = j + dj[dir];
		if (valid_indices(ii, jj)){
		  if (1 + dist_no_fall[ii][jj][k][l] == dist_no_fall[i][j][k][l])
		    nextmove[i][j][k][l].push_back(delta[dir]);
		}
	      }
	    }
}

// Never dig a hole. Never fall.
void Board::SS_BFS_no_fall(int r,int c,int dist[HEIGHT][WIDTH]) {
  queue<pii> q;

  dist[r][c] = 0;
  q.push(make_pair(r,c));
  
  while(!q.empty()){
    pii t = q.front(); q.pop();
    int I = t.first, J = t.second;
    
    vector<cmd> v = valid_moves(I,J,false);
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(canStand(nI,nJ) && dist[nI][nJ] == oo){
	dist[nI][nJ] = dist[I][J] + 1;
	q.push(make_pair(nI,nJ));
      }
    }
  }
}

void Board::BFS_with_fall(int delay){
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      for(int k=0;k<HEIGHT;k++)
        for(int l=0;l<WIDTH;l++)
          dist_with_fall[i][j][k][l] = oo;   // Not reachable

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      SS_BFS_with_fall(i,j,delay,dist_with_fall[i][j]);
}

// Calculate the distance if we are allowed to fall. If we are "inside" of a
//  brick then we will fall. Note that this means that this brick must have
//  been broken.
// Moreover, this will break bricks...
void Board::SS_BFS_with_fall(int r,int c,int delay,int dist[HEIGHT][WIDTH]){
  queue<tuple<int,int,int> > q;
  
  int dist2[HEIGHT][WIDTH][11];
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      for(int k=0;k<=10;k++)
	dist2[i][j][k] = oo;
  dist2[r][c][delay] = 0;
  q.push(make_tuple(r,c,delay));
  
  int I,J,D;
  while(!q.empty()){
    tie(I,J,D) = q.front(); q.pop();
        
    vector<cmd> v = valid_moves(I,J,(D == 0));
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      int nD = max(0,(v[k] == DIG_LEFT || v[k] == DIG_RIGHT ? 8 : D-1));
      nD = (nD+2)/3;
      if(valid_indices(nI,nJ) && dist2[nI][nJ][nD/3] == oo){
	dist2[nI][nJ][nD] = dist2[I][J][D] + 1;
	q.push(make_tuple(nI,nJ,nD));
      }
    }
  }
  
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++)
      dist[i][j] = *min_element(dist2[i][j],dist2[i][j]+11);
}

void Board::init(int delay) {
  BFS_no_fall();
  BFS_with_fall(delay);
}

#endif
