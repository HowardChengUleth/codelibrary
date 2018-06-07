#include <iostream>
#include <string>
#include <sstream>
#include "util.h"
#include <cstdlib>

using namespace std;

int main()
{
  cout << "Running init" << endl;
  system("init.sh");

  for (int g = 0; g < 10; g++) {
    for (int i = 0; i < DEF_POP_SIZE; i++) {
      ostringstream oss;
      oss << "rungame " << i;
      
      cout << "Generation " << g+1 << ":  Running game " << i << endl;
      system(oss.str().c_str());
    }
    system("nextgen alpha");
  }
  
  return 0;
}
