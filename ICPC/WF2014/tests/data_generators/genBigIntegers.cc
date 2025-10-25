#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int NUM_NUMBERS = 100000;
const int MAX_DIGS =  40;

int randInt(int min=1,int max=MAX_DIGS){
  int x = rand() % (max-min);
  if(x < 0) x += (max-min);
  return x+min;
}

int main(){
  srand( time(NULL) );

  for(int i=0;i<NUM_NUMBERS;i++){
    int digs = randInt();
    bool neg = false;
    if(rand() % 2 == 0) { neg = true; cout << "-"; }
    for(int j=0;j<digs || neg;j++){
      int x = (rand() % 10);
      cout << x;
      if(x != 0) neg = false;
    }
    cout << endl;
  }
  return 0;
}
