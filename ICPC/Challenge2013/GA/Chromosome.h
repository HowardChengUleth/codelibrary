#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <iostream>
#include <vector>
#include "util.h"

using namespace std;

struct Chromosome
{
  vector<double> alpha;
  double total_score;
  int num_games;

  Chromosome(int size = 0);

  void resize(int size);

  void mutate();
  
  // crossover: pick a random switch point and take first part from one
  //            and second part from the other
  Chromosome operator*(const Chromosome &c) const;

  void add_score(int my_score, int opp_score);

  double score() const
  {
    return (num_games) ? total_score / num_games : 0;
  }
  
  bool operator<(const Chromosome &c) const
  {
    return score() < c.score();
  }

  static int read_num_weights(int argc, char *argv[]);


};

istream &operator>>(istream &is, Chromosome &c);
ostream &operator<<(ostream &os, const Chromosome &c);


#endif
