// Rubik's Cube: This is how the cube labelled. Note that you fold the box away
// from you, so C is the closest face to you and E is the farthest:
//
//         +-------+                            +-------+
//         |       |                            | 1 2 3 |
//         |   A   |                            | 8 0 4 |
//         |       |                            | 7 6 5 |
// +-------+-------+-------+-------+    +-------+-------+-------+-------+
// |       |       |       |       |    | 1 2 3 | 1 2 3 | 1 2 3 | 1 2 3 |
// |   B   |   C   |   D   |   E   |    | 8 0 4 | 8 0 4 | 8 0 4 | 8 0 4 |
// |       |       |       |       |    | 7 6 5 | 7 6 5 | 7 6 5 | 7 6 5 |
// +-------+-------+-------+-------+    +-------+-------+-------+-------+
//         |       |                            | 1 2 3 |
//         |   F   |                            | 8 0 4 |
//         |       |                            | 7 6 5 |
//         +-------+                            +-------+
//
//   cube[i][j] is the color of index j on face (i+'A')
// To rotate Face X 90 degrees clockwise, call rotateFace(X)
// To rotate Face X 90 degrees counterclockwise, call rotateFace(X) 3 times

string rot[6] = {"BEDC1111","ACFE7773","ADFB5713","AEFC3733","ABFD1753","BCDE5555"};
int cube[6][9],t[3]; // t is a tmp variable
int m9(int x){ return (x % 9 == 0 ? 1 : x % 9); }

void rotateFace(char F){
   int ind = F - 'A'; rotate(cube[ind]+1,cube[ind]+7,cube[ind]+9);
   
   string r = rot[ind];
   for(int i=0;i<3;i++) t[i] = cube[r[3]-'A'][m9(r[7]-'0'+i)];

   for(int i=7;i>4;i--) for(int j=0;j<3;j++)
	 cube[r[i-4]-'A'][m9(r[i]-'0'+j)] = cube[r[i-5]-'A'][m9(r[i-1]-'0'+j)];
   for(int j=0;j<3;j++) cube[r[0]-'A'][m9(r[4]-'0'+j)] = t[j];
}

void printCube(){
   string o = "123804765";
   for(int F=0;F<6;F++){
      cout << "Face " << char(F+'A') << endl;
      for(int i=0;i<3;i++){
	 for(int j=0;j<3;j++) cout << setw(3) << cube[F][o[i*3+j]-'0'];
	 cout << endl;
      }
   }
}
