#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "util.h"
#include "Chromosome.h"

#include <sys/types.h>
#include <dirent.h>

using namespace std;

int pop_size, keep_size, games_per_alpha;
double mut_rate;

void usage(int argc, char *argv[])
{
  cerr << "Usage: " << argv[0] << " alpha_file [-p population_size] [-m mutation_rate] [-t number_to_keep] [-g games_per_alpha]" << endl;
  exit(1);
}

void read_params(int argc, char *argv[])
{
  pop_size = DEF_POP_SIZE;
  mut_rate = DEF_MUT_RATE;
  keep_size = DEF_KEEP;
  games_per_alpha = DEF_GAMES_PER_ALPHA;
  for (int i = 2; i < argc; i++) {
    string arg = argv[i];
    if (arg == "-p") {
      if (i+1 >= argc) usage(argc, argv);
      pop_size = atoi(argv[i+1]);
    } else if (arg == "-m") {
      if (i+1 >= argc) usage(argc, argv);
      mut_rate = atof(argv[i+1]);
    } else if (arg == "-t") {
      if (i+1 >= argc) usage(argc, argv);
      keep_size = atoi(argv[i+1]);
    } else if (arg == "-g") {
      if (i+1 >= argc) usage(argc, argv);
      games_per_alpha = atoi(argv[i+1]);
    }
  }
}

string weight_name(int i)
{
  ostringstream oss;
  oss << ALPHA_DIR << "alpha." << i;
  return oss.str();
}

vector<string> get_map_list()
{
  vector<string> list;

  DIR *dir = opendir(MAP_DIR.c_str());
  if (!dir) {
    cerr << "Cannot open map directory " << MAP_DIR << endl;
    exit(1);
  }

  struct dirent *ent;
  while ((ent = readdir(dir))) {
    if (ent->d_name[0] != '.') {
      list.push_back(string(ent->d_name));
    }
  }

  closedir(dir);
  sort(list.begin(), list.end());
  return list;
}

void write_population(const vector<Chromosome> &population)
{
  for (unsigned int i = 0; i < population.size(); i++) {
    string fname = weight_name(i);
    ofstream out(fname.c_str());
    if (!out) {
      cerr << "Cannot open output file " << fname << endl;
      exit(1);
    }

    out << population[i];
  }
}

void make_tournament(const vector<Chromosome> &population)
{
  string prefix = TOURN_DIR + TOURN_SCHED + ".";
  
  vector<string> maps = get_map_list();
  for (int i = 0; i < pop_size; i++) {
    for (int match = 0; match < games_per_alpha; match++) {
      int opp;
      if (match < 2) {
	opp = pop_size - 1 - (rand() % keep_size);
      } else {
	opp = rand() % pop_size;
      }
      while (opp == i) {
	if (match < 2) {
	  opp = pop_size - 1 - (rand() % keep_size);
	} else {
	  opp = rand() % pop_size;
	}
      }
      ostringstream oss;
      oss << prefix << i << "-" << match;
      ofstream schedule(oss.str().c_str());
      if (!schedule) {
	cerr << "Cannot open output file " << oss.str() << endl;
	exit(1);
      }
    
      for (unsigned int j = 0; j < maps.size(); j++) {
	schedule << weight_name(i) << ' '
		 << weight_name(opp) << ' '
		 << MAP_DIR << maps[j] << endl;
      }
    }
  }
}
