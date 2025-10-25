#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int NUM_POINTS = 10;
const int MINI = -1000000;
const int MAXI =  1000000;

double randDouble(int min=MINI,int max=MAXI){
  return (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
}

int randInt(int min=MINI,int max=MAXI){
  int x = rand() % (MAXI-MINI);
  return x+MINI;
}

int main(){
  cout << fixed << setprecision(5);
  for(int i=0;i<NUM_POINTS;i++)
    cout << randDouble() << " " << randDouble() << endl;
    //cout << randInt() << " " << randInt() << endl;
  return 0;
}
