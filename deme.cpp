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
    for (auto &chr: pop_) {
        chr = new ClimbChromosome(cities_ptr); //keep this fs, yeet all other climbchromosome
    }
}

// Clean up as necessary
Deme::~Deme() {
    for (auto &chr: pop_) {
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
void Deme::compute_next_generation() {
    vector<Chromosome *> next_gen{};
    next_gen.reserve(pop_.size());

    for (int i = 0; i < trunc(pop_.size() / 2); i++) {
        generator_.seed(chrono::system_clock::now().time_since_epoch().count());
        unsigned int rand_a = (generator_() % 100) + 1;//1-100
        generator_.seed(chrono::system_clock::now().time_since_epoch().count());
        unsigned int rand_b = (generator_() % 100) + 1;//1-100

        double rand_percentage_a = rand_a / 100;
        double rand_percentage_b = rand_b / 100;

        auto chr_a = select_parent();
        auto chr_b = select_parent();
        if (rand_percentage_a <= mut_rate_) {
            chr_a->mutate();
        }
        if (rand_percentage_b <= mut_rate_) {
            chr_b->mutate();
        }
        auto offspring = chr_a->recombine(chr_b);


        next_gen.push_back(offspring.first);
        next_gen.push_back(offspring.second);

    }
    pop_.swap(next_gen); //replace population

    for (auto &chr: next_gen) {
        delete chr;
    }

}

// Return a copy of the chromosome with the highest fitness.
const Chromosome *Deme::get_best() const {
    double best_fit = 0.0;
    Chromosome *best = nullptr;
    for (const auto &chr: pop_) {
        double chr_fitness = chr->get_fitness();
        if (best_fit < chr_fitness) {
            best = chr;
            best_fit = chr_fitness;
        }
    }
    return best;
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome. The selection should favor fitter parents.
// https://en.wikipedia.org/wiki/Fitness_proportionate_selection
// if fitness' are 60 for a and 40 for b, there should be a 60% chance of a and 40% chance of b, etc
Chromosome *Deme::select_parent() {
    //sum fitnesses
    double total_fit = 0.0;
    vector<double> fits; //fits is all fitnesses in same order as pop_; selecting fits[i] gets fitness @ i while pop_[i] gets the chr
    for (const auto &chr: pop_) {
        total_fit += chr->get_fitness();
        fits.push_back(chr->get_fitness());
    }
    generator_.seed(chrono::system_clock::now().time_since_epoch().count()); //use curr time as seed
    //generator_.min()
    //randomly gen number from 0/1 to total_fit
    int int_total_fit = trunc(total_fit);
    int rand_num = generator_() % int_total_fit;
    for (auto i = 0; i < trunc(fits.size()); i++) {//roulette wheel time!
        rand_num -= fits[i];
        if (rand_num < 0) {
            return pop_[i];
        }

    }
    return pop_[0]; //should never hit this
}
