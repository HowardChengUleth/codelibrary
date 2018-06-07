#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include "util.h"
#include "Chromosome.h"

using namespace std;


int player_num(const string &fname)
{
  int prefix_len = weight_name(0).length() - 1;
  string suffix = fname.substr(prefix_len);
  return atoi(suffix.c_str());
}

void read_results(vector<Chromosome> &population)
{
  string prefix = TOURN_DIR + RESULT + ".";
  for (unsigned int i = 0; i < population.size(); i++) {
    for (int match = 0; match < games_per_alpha; match++) {
      ostringstream oss;
      oss << prefix << i << "-" << match;
      ifstream in(oss.str().c_str());
      if (!in) {
	cerr << "Cannot open input file " << oss.str() << endl;
	exit(1);
      }
      
      string player1, player2;
      int score1, score2;
      while (in >> player1 >> player2 >> score1 >> score2) {
	int i1 = player_num(player1);
	int i2 = player_num(player2);
	
	population[i1].add_score(score1, score2);
	population[i2].add_score(score2, score1);
      }
    }
  }
}

void next_gen(vector<Chromosome> &population)
{
  sort(population.begin(), population.end());

  int child_size = pop_size - keep_size;
  int newsize = min(child_size, (int)floor(pop_size * 0.10+ 0.5));

  int gene_size = population[0].alpha.size();
  for (int i = 0; i < newsize; i++) {
    population[i] = Chromosome(gene_size);
  }
  
  for (int i = newsize; i < child_size; i++) {
    int p1 = rand() % keep_size;
    int p2 = rand() % keep_size;
    while (p2 == p1) {
      p2 = rand() % keep_size;
    }
    p1 += child_size;
    p2 += child_size;
    
    population[i] = population[p1] * population[p2];

    if ((double)rand()/RAND_MAX <= mut_rate) {
      population[i].mutate();
    }
  }
  write_population(population);

  fstream out;
  out.open("progress.txt", ios_base::out | ios_base::app);
  out << "top score = " << population[pop_size-1].score() << endl;
  out << "alpha = " << endl << population[pop_size-1];
  out << "=======" << endl;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  read_params(argc, argv);

  vector<Chromosome> population(pop_size);
  for (int i = 0; i < pop_size; i++) {
    string fname = weight_name(i);
    ifstream in(fname.c_str());
    if (!in) {
      cerr << "Cannot open input file " << fname << endl;
      exit(1);
    }

    in >> population[i];
  }

  read_results(population);

  next_gen(population);
  
  make_tournament(population);
  
  return 0;
}
