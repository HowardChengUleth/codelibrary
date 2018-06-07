// Prints n followed by n^2 integers

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int NUM_DATA_SETS = 15000;
const int MAX_NODES = 40;
const int PERCENT_DENSITY = 10;
const int MINI = 0;
const int MAXI = PERCENT_DENSITY+1;

int randInt(int min=MINI,int max=MAXI){
  int x = rand() % (max-min+1);
  return x+min;
}

int main(){
  for(int k=0;k<NUM_DATA_SETS;k++){
    int m = randInt(3,MAX_NODES);
    int n = randInt(3,MAX_NODES);
    cout << m << " " << n << endl;
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
	cout << (randInt() == 0) << " ";
    cout << endl;
  }
  return 0;
}
