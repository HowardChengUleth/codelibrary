#ifndef __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__
#define __UOFL_ICPC_CHALLENGE_UTILITIES_BOARD__

#include <vector>

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

bool visited_[HEIGHT][WIDTH];

// Information about the game board (no information about the players/enemies)
class Board{
  private:
  Item A[HEIGHT][WIDTH];
  int dist_no_fall[HEIGHT][WIDTH][HEIGHT][WIDTH];
  int dist_with_fall[HEIGHT][WIDTH];
  UnionFind uf;

  void BFS_no_fall();
  void SS_BFS_no_fall(int r,int c,int dist[HEIGHT][WIDTH]);
  
  void SS_BFS_with_fall(int r,int c,int delay);

  int uf_index(int r, int c) const
  {
    return r * WIDTH + c;
  }

  public:
  Board() : uf(HEIGHT*WIDTH) { }

  int scc_index(int r, int c) 
  {
    return uf.find(uf_index(r, c));
  }
  
  vector<cmd> valid_moves(int row,int col,bool canDig) const;

  // Read in the board
  void read();
  
  // Initializes the board before anyone is on it
  void init(int r,int c,int delay);
  
  // Determines if you can stand on a location.
  //  -- Only considers the map layout, not Enemies.
  bool canStand(int ROW,int COL) const;

  Item itemAt(int i,int j) const{
    return (valid_indices(i,j) ? A[i][j] : EMPTY);
  }
  
  // I'm letting my friend touch my private parts... This is all that I remember
  //  from CS 2620 ... Thanks Howard...
  friend int distance(int r1,int c1,int r2,int c2);
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
  if(!canStand(row,col)) return vector<cmd>(1,DOWN); // Must be falling! DOWN for a reason... I don't remember what that reason is...
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

  // This gross mess updates nextmove_dont_use_me_use_Goto_instead[][][][]
  for (int i = 0; i < HEIGHT; i++) 
    for (int j = 0; j < WIDTH; j++)
      if (canStand(i,j)) 
	for (int k = 0; k < HEIGHT; k++) 
	  for (int l = 0; l < WIDTH; l++)
	    if (canStand(k,l)) {
	      nextmove_dont_use_me_use_Goto_instead[i][j][k][l].clear();
	      if (k == i && j == l) {
		nextmove_dont_use_me_use_Goto_instead[i][j][k][l].push_back(NONE); continue;
	      }
	      for (int dir = 0; dir < 4; dir++) {
		int ii = i + di[dir], jj = j + dj[dir];
		if (valid_indices(ii, jj)){
		  if (1 + dist_no_fall[ii][jj][k][l] == dist_no_fall[i][j][k][l])
		    nextmove_dont_use_me_use_Goto_instead[i][j][k][l].push_back(delta[dir]);
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

// Calculate the distance if we are allowed to fall. If we are "inside" of a
//  brick then we will fall. Note that this means that this brick must have
//  been broken.
// Moreover, this will break bricks...
void Board::SS_BFS_with_fall(int r,int c,int delay){
  typedef tuple<int,int,int,int,cmd> tiiiic; // Like the typedef name?
  priority_queue<tiiiic,vector<tiiiic>,greater<tiiiic> > pq;
  
  vector<cmd> moves[HEIGHT][WIDTH];
  int dist2[HEIGHT][WIDTH][11];
  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      for(int k=0;k<=10;k++) dist2[i][j][k] = oo;
      dist_with_fall[i][j] = oo;
    }

  dist2[r][c][delay] = dist_with_fall[r][c] = 0;
  moves[r][c].push_back(NONE);
  pq.push(make_tuple(0,r,c,delay,NONE));
  
  int I,J,D,myDist;
  cmd C;
  for(bool first=true;!pq.empty();first=false){
    tie(myDist,I,J,D,C) = pq.top(); pq.pop();

    if(myDist > dist2[I][J][D] && !first) continue;
        
    vector<cmd> v = valid_moves(I,J,(D == 0));
    for(size_t k=0;k<v.size();k++){
      int nI = I+di[v[k]], nJ = J+dj[v[k]];
      if(!valid_indices(nI,nJ)) continue;
      
      // The next line is 8 because:
      //  first you dig (at turn t), then move left (at turn t+1), then down (at turn t+2)
      int nD = max(0,(v[k] == DIG_LEFT || v[k] == DIG_RIGHT ? 8 : D-1));
      int nDist = myDist + dijkstra_dist[v[k]];
      cmd nC = (first ? v[k] : C);
      
      if(dist2[nI][nJ][nD] < nDist) continue; // We already have better!
      
      if(dist_with_fall[nI][nJ] == nDist){ // Add this move to our list!
	if(find(moves[nI][nJ].begin(),moves[nI][nJ].end(),nC) == moves[nI][nJ].end())
	  moves[nI][nJ].push_back(nC);
      }
      if(dist_with_fall[nI][nJ] > nDist){
	dist_with_fall[nI][nJ] = nDist;
	moves[nI][nJ].clear();       // Remove old results
	moves[nI][nJ].push_back(nC); // Put me in!
      }
      
      if(dist2[nI][nJ][nD] > nDist){
	dist2[nI][nJ][nD] = nDist;
	pq.push(make_tuple(nDist,nI,nJ,nD,nC));
      }
    }
  }

  for(int i=0;i<HEIGHT;i++)
    for(int j=0;j<WIDTH;j++){
      sort(moves[i][j].begin(),moves[i][j].end());
      nextmove_dont_use_me_use_Goto_instead[r][c][i][j] = moves[i][j];
    }
  
}

void Board::init(int r,int c,int delay) {
  if(r == -1 && c == -1) return; // Doesn't matter, I'm dead...
  BFS_no_fall();
  SS_BFS_with_fall(r,c,delay);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      for (int k = 0; k < HEIGHT; k++) {
	for (int l = 0; l < WIDTH; l++) {
	  if (dist_no_fall[i][j][k][l] < oo &&
	      dist_no_fall[k][l][i][j] < oo) {
	    uf.merge(uf_index(i,j), uf_index(k,l));
	  }
	}
      }
    }
  }
}

#endif
