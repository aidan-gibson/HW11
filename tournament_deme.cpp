//
// Created by Aidan Gibson on 4/22/2022.
//

#include "tournament_deme.h"
#include <cassert> //for linux compat
#include <chrono> //for linux compat
using namespace std;

TournamentDeme::TournamentDeme(const Cities *cities_ptr, unsigned int pop_size, double mut_rate) : Deme(cities_ptr,
                                                                                                        pop_size,
                                                                                                        mut_rate) {
  for (auto &chr : pop_) {
    chr = new ClimbChromosome(cities_ptr); //keep this fs, yeet all other climbchromosome
  }
}
Chromosome *TournamentDeme::select_parent() {
  //select P parents at random from pop_, where P is some const pwr of 2
  unsigned int rand_pow_max = trunc(log2(pop_.size()));
  generator_.seed(chrono::system_clock::now().time_since_epoch().count());
  unsigned int rand_pow = (generator_() % rand_pow_max)+1;//[1,rand_pow_max]
  unsigned int P = pow(2,rand_pow);
  //I have P (total parent count), but I must now select P random parents from pop_
  auto pop_copy = pop_;
  vector<Chromosome *> selected_pop = {};
  for (unsigned int i = 0; i<P; i++){ //P times
    //randomly select a parent from pop_copy and pop them off of pop_copy. add them to selected_pop
    generator_.seed(chrono::system_clock::now().time_since_epoch().count());
    unsigned int rand_pop_index = generator_() % pop_copy.size(); //select random parent from pop_copy
    auto it = selected_pop.begin()+rand_pop_index; //turns rand_pop_index into an iterator so .erase() will work.
    selected_pop.push_back(pop_copy[rand_pop_index]);//add them to selected_pop
    pop_copy.erase(it);//remove them from pop_copy
  }
  assert(selected_pop.size()==P);
  //now selected_pop fights
  while(selected_pop.size()>1){
    if(selected_pop[0]->get_fitness()<selected_pop[1]->get_fitness()){
      selected_pop.erase(selected_pop.begin());
    }
    else{
      selected_pop.erase(selected_pop.begin()+1);
    }
  }
  assert(selected_pop.size()==1);
  return selected_pop[0];
}
