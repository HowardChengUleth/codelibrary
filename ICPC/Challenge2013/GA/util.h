#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

const string ALPHA_DIR = "alphadir/";
const string TOURN_DIR = "games/";
const string TOURN_SCHED = "games";
const string MAP_DIR = "maps/";
const string RESULT = "results";
const string CODERUNNER = "../coderunner.jar";
const string PLAYER = "uleth";

const int DEF_POP_SIZE = 100;
const double DEF_MUT_RATE = 0.25;
const int DEF_KEEP = 25;
const int DEF_GAMES_PER_ALPHA = 10;

extern int pop_size, keep_size, games_per_alpha;
extern double mut_rate;

void usage(int argc, char *argv[]);
void read_params(int argc, char *argv[]);

string weight_name(int i);

vector<string> get_map_list();

struct Chromosome;

void write_population(const vector<Chromosome> &population);

void make_tournament(const vector<Chromosome> &population);

#endif
