#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

#include "Chromosome.h"

using namespace std;

Chromosome::Chromosome(int size)
  : total_score(0.0), num_games(0)
{
  resize(size);
}

void Chromosome::resize(int size)
{
  alpha.resize(size);
  for (int i = 0; i < size; i++)
    alpha[i] = (double)rand() / RAND_MAX * 1.2 - 0.2;
}

void Chromosome::mutate()
{
  // two types of mutations
  if (rand() % 2) {
    // pick two random weights and swap them
    int i = rand() % alpha.size();
    int j = rand() % alpha.size();
    while (i == j) {
      j = rand() % alpha.size();
    }
    swap(alpha[i], alpha[j]);
  } else {
    // pick a random weight and adjust slightly
    int i = rand() % alpha.size();
    alpha[i] += ((rand() % 2) ? -1 : 1) * (0.25*alpha[i]);
  }
}
  
// crossover: pick a part from one or the other
Chromosome Chromosome::operator*(const Chromosome &c) const
{
  Chromosome child(alpha.size());
  for (unsigned int i = 0; i < alpha.size(); i++) {
    child.alpha[i] = (rand() % 2) ? alpha[i] : c.alpha[i];
  }
  return child;
}

void Chromosome::add_score(int my_score, int opp_score)
{
  num_games++;
  total_score += my_score;
}

int Chromosome::read_num_weights(int argc, char *argv[])
{
  if (argc < 2) usage(argc, argv);
  
  fstream fin(argv[1]);
  if (!fin.is_open()) {
    cerr << "Cannot open file -- Exiting." << endl;
    cerr << "Expecting: " << argv[1] << endl;
    exit(1);
  }
  
  int c = 0;
  double tmp;
  while (fin >> tmp) c++;
  return c;
}

istream &operator>>(istream &is, Chromosome &c)
{
  c.total_score = 0.0;
  c.num_games = 0.0;
  c.alpha.resize(0);
  double x;
  while (is >> x) {
    c.alpha.push_back(x);
  }
  return is;
}

ostream &operator<<(ostream &os, const Chromosome &c)
{
  for (unsigned int i = 0; i < c.alpha.size(); i++) {
    os << fixed << setprecision(6) << c.alpha[i] << endl;
  }
  return os;
}
