#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include "util.h"
#include <cstdio>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " game_num" << endl;
    exit(1);
  }

  ifstream gafile("ga");
  if (!gafile) {
    cerr << "Cannot open input file ga" << endl;
    exit(1);
  }
  string line;
  int games_per_alpha;
  while (getline(gafile, line)) {
    if (line.find("GAMES_PER_ALPHA") == 0) {
      sscanf(line.c_str(), "GAMES_PER_ALPHA=%d", &games_per_alpha);
      break;
    }
  }
  

  int game = atoi(argv[1]);
  int alpha = game / games_per_alpha;
  int match = game % games_per_alpha;

  ostringstream oss1, oss2;
  oss1 << TOURN_DIR << TOURN_SCHED << "." << alpha << "-" << match;
  oss2 << TOURN_DIR << RESULT << "." << alpha << "-" << match;
  
  string game_file = oss1.str();
  string result_file = oss2.str();
  
  ifstream in(game_file.c_str());
  ofstream out(result_file.c_str());
  if (!in) {
    cerr << "Cannot open input file " << game_file << endl;
    exit(1);
  }
  if (!out) {
    cerr << "Cannot open output file " << result_file << endl;
    exit(1);
  }

  string alpha1, alpha2, map;
  while (in >> alpha1 >> alpha2 >> map) {
    string cmd = "java -XX:ParallelGCThreads=2 -Xms32m -Xmx1g -jar ";
    cmd += CODERUNNER;
    cmd += " -player syncpipe 2 ";
    cmd += PLAYER + " " + alpha1;
    cmd += " -player syncpipe 2 ";
    cmd += PLAYER + " " + alpha2;
    cmd += " -map " + map;
    cmd += " -duration 750";
    cmd += " -view turns /dev/null 2> /dev/null";

    cout << "Running " << alpha1 << " vs " << alpha2 << " on " << map << endl;

    FILE *p = popen(cmd.c_str(), "r");
    if (!p) {
      cerr << "Unable to start command: " << cmd << endl;
      exit(1);
    }
    char line[10000];
    int score1, score2;
    while (fgets(line, 10000, p)) {
      if (sscanf(line, "Score: %d vs %d", &score1, &score2) == 2) {
	out << alpha1 << ' ' << alpha2 << ' ' << score1 << ' ' << score2 << endl;
      }
    }

    pclose(p);
  }
  
  return 0;
}
