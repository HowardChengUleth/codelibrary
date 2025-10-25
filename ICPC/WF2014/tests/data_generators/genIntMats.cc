// Prints n followed by n^2 integers

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

const int NUM_DATA_SETS = 10;
const int MINI = -1000000;
const int MAXI =  1000000;

int randInt(int min=MINI,int max=MAXI){
  int x = rand() % (max-min);
  return x+min;
}

int main(){
  for(int k=0;k<NUM_DATA_SETS;k++){
    int n = randInt(5,100);
    cout << n << endl;
    for(int i=0;i<n;i++)
      for(int j=0;j<n;j++)
	cout << randInt() << " ";
    cout << endl;
  }
  return 0;
}
