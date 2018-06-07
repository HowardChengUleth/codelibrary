#include <iostream>
#include <string>
#include <sstream>
#include "util.h"
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

pid_t spawn(int g, int start, int factor)
{
  pid_t pid = fork();
  if (pid == -1) {
    exit(-1);
  }
  if (pid) {
    return pid;
  }
  for (int i = start; i < DEF_POP_SIZE; i += factor) {
    ostringstream oss;
    oss << "rungame " << i;
    
    cout << "Generation " << g+1 << ":  Running game " << i << endl;
    system(oss.str().c_str());
  }
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " factor" << endl;
    return 1;
  }

  int factor = atoi(argv[1]);
  cout << "Running init" << endl;
  system("cp -f ../Player/alpha ./");
  system("cp -f ../Player/uleth ./");
  system("rm -f games/* alphadir/*");
  system("init alpha");

  for (int g = 0; g < 10; g++) {
    vector<pid_t> children;
    for (int i = 0; i < factor; i++) {
      children.push_back(spawn(g, i, factor));
    }

    while (children.size() > 0) {
      if (children[children.size()-1] != 
	  waitpid(children[children.size()-1], NULL, 0)) {
	cerr << "Wait error" << endl;
	exit(1);
      }
      children.pop_back();
    }

    system("nextgen alpha");
  }
  
  return 0;
}
