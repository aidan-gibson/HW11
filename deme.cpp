/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "climb_chromosome.h"
#include "deme.h"
#include <chrono> //for linux compat

using namespace std;

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities *cities_ptr, unsigned pop_size, double mut_rate)
    : pop_(pop_size), mut_rate_(mut_rate), generator_(rand()) {
  for (auto &chr : pop_) {
    chr = new ClimbChromosome(cities_ptr); //keep this fs, yeet all other climbchromosome
  }
}

// Clean up as necessary
Deme::~Deme() {
  for (auto &chr : pop_) {
    delete chr;
  }
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
//
//
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
//void Deme::compute_next_generation() {
//  vector<Chromosome *> next_gen{};
//  next_gen.reserve(pop_.size());
//
//  for (int i = 0; i < trunc(pop_.size() / 2); i++) {
//    generator_.seed(chrono::system_clock::now().time_since_epoch().count());
//    unsigned int rand_a = (generator_() % 100) + 1;//1-100
//    generator_.seed(chrono::system_clock::now().time_since_epoch().count());
//    unsigned int rand_b = (generator_() % 100) + 1;//1-100
//
//    double rand_percentage_a = rand_a / 100;
//    double rand_percentage_b = rand_b / 100;
//
//    auto chr_a = select_parent();
//    auto chr_b = select_parent();
//    if (rand_percentage_a <= mut_rate_) {
//      chr_a->mutate();
//    }
//    if (rand_percentage_b <= mut_rate_) {
//      chr_b->mutate();
//    }
//    auto offspring = chr_a->recombine(chr_b);
//
//    next_gen.push_back(offspring.first);
//    next_gen.push_back(offspring.second);
//
//  }
//  pop_.swap(next_gen); //replace population
//
//  for (auto &chr : next_gen) {
//    delete chr;
//  }
//
//}
void Deme::compute_next_generation() //Eitan solution
{
  auto newpop = pop_;
  assert(pop_.size() % 2 == 0 && "Even population size required!");

  for (unsigned i = 0; i < pop_.size(); ) {
    auto p1 = select_parent();
    auto p2 = select_parent();

    static std::uniform_real_distribution<> dist(0.0, 1.0);
    if (dist(generator_) <= mut_rate_) {
      p1->mutate();
    }
    if (dist(generator_) <= mut_rate_) {
      p2->mutate();
    }

    auto children = p1->recombine(p2);
    newpop[i++] = children.first;
    newpop[i++] = children.second;
  }

  for (auto cp : pop_) {
    delete cp;
  }
  swap(pop_, newpop);
}
// Return a copy of the chromosome with the highest fitness.
//const Chromosome *Deme::get_best() const {
//  double best_fit = 0.0;
//  Chromosome *best = nullptr;
//  for (const auto &chr : pop_) {
//    double chr_fitness = chr->get_fitness();
//    if (best_fit < chr_fitness) {
//      best = chr;
//      best_fit = chr_fitness;
//    }
//  }
//  return best;
//}
const Chromosome* Deme::get_best() const // Eitan solution
{
  assert(!pop_.empty());
  return *std::max_element(pop_.cbegin(), pop_.cend(), [](auto cp1, auto cp2){
    return cp1->get_fitness() < cp2->get_fitness(); });
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome. The selection should favor fitter parents.
// https://en.wikipedia.org/wiki/Fitness_proportionate_selection
// if fitness' are 60 for a and 40 for b, there should be a 60% chance of a and 40% chance of b, etc
//Chromosome *Deme::select_parent() {
//  //sum fitnesses
//  double total_fit = 0.0;
//  vector<double>
//      fits; //fits is all fitnesses in same order as pop_; selecting fits[i] gets fitness @ i while pop_[i] gets the chr
//  for (const auto &chr : pop_) {
//    total_fit += chr->get_fitness();
//    fits.push_back(chr->get_fitness());
//  }
//  generator_.seed(chrono::system_clock::now().time_since_epoch().count()); //use curr time as seed
//  //generator_.min()
//  //randomly gen number from 0/1 to total_fit
//  int int_total_fit = trunc(total_fit);
//  int rand_num = generator_() % int_total_fit;
//  for (auto i = 0; i < trunc(fits.size()); i++) {//roulette wheel time!
//    rand_num -= fits[i]; //TODO boof and questionable
//    if (rand_num < 0) {
//      return pop_[i];
//    }
//
//  }
//  return pop_[0]; //should never hit this
//}
Chromosome* Deme::select_parent() //Eitan solution
{
  // Figure out what the total sum of fitness in pop_ is:
  const double total_fitness = std::accumulate(pop_.cbegin(), pop_.cend(), 0.,
                                               [](double sum, auto cp){ return sum + cp->get_fitness(); });

  // Pick a random fitness sum value to stop the wheel at:
  static std::uniform_real_distribution<> dist(0.0, 1);
  const double threshold = total_fitness * dist(generator_);

  // Now, find the first chromosome whose partial sum of fitness exceeds threshold:
  double fit_sum = 0.; // Total fitness so far

  const auto it = std::find_if(pop_.cbegin(), pop_.cend(), [&](auto cp) {
    fit_sum += cp->get_fitness();
    return fit_sum >= threshold;
  });
  assert(it != pop_.cend());

  return *it;
}