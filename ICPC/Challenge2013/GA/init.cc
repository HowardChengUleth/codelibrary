#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include "util.h"
#include "Chromosome.h"

using namespace std;



int main(int argc, char *argv[])
{
  srand(time(NULL));

  int num_weights = Chromosome::read_num_weights(argc, argv);
  read_params(argc, argv);

  vector<Chromosome> population(pop_size);
  for (int i = 0; i < pop_size; i++) {
    population[i].resize(num_weights);
  }

  write_population(population);
  make_tournament(population);

  ofstream R("progress.txt");
  return 0;
}
