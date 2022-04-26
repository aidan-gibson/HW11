
#include "climb_chromosome.h"

using namespace std;

/*
 * Override mutate in favor of "local hill climbing"
 * Search for best solution in local "neighborhood"
 *
 * Assuming a chromosome w N genes (cities)
 *
 * Eval fitness of current/orig chromosome
 *
 * Pick random pt
 *
 * Swap city @ index's [p] and [p-1], eval resulting fitness
 * if rand_pt==0, swap w [N-1] instead of [index-1]
 *
 * Swap city [p] (from orig chr) w city[p+1], then eval resulting chr fitness.
 * if p==N-1, swap w city[0] instead of [N]
 *
 * find which of the three chromosomes produced the fittest ordering and choose that fittest chromosome as the resulting mutated chromosome.
 */
void ClimbChromosome::mutate() {
    //eval fitness of original/current chr
    double orig_chr_fitness = get_fitness();
    auto orig_order = order_;
    Cities::permutation_t swap_a_order = order_;
    Cities::permutation_t swap_b_order = order_;

    //pick rand pt
    unsigned int rand_pt = generator_()% (order_.size());
    //If p==0, swap with the city at index N-1 instead of at index -1
    if (rand_pt == 0){
        swap(swap_a_order[rand_pt], swap_a_order[swap_a_order.size()-1]);
    }
    else {
        //swap city[p] w city[p-1]
        swap(swap_a_order[rand_pt], swap_a_order[rand_pt - 1]);
    }
    order_ = swap_a_order;
    assert(is_valid()); //swapped order_ should be valid!
    double swapped_chr_fitness_a = get_fitness();

    if (rand_pt == swap_b_order.size()-1) {
        swap(swap_b_order[rand_pt], swap_b_order[0]);
    }
    else {
        swap(swap_b_order[rand_pt], swap_b_order[rand_pt + 1]);
    }

    order_ = swap_b_order;
    assert(is_valid()); //swapped order_ should be valid!
    double swapped_chr_fitness_b = get_fitness();

    //Find which of the three chromosomes produced the fittest ordering and choose that fittest chromosome as the resulting mutated chromosome.

    if (orig_chr_fitness>=swapped_chr_fitness_a && orig_chr_fitness>=swapped_chr_fitness_b) {
        order_ = orig_order;
    }
    else if (swapped_chr_fitness_a>=orig_chr_fitness && swapped_chr_fitness_a>=swapped_chr_fitness_b) {
        order_ = swap_a_order;
    }
    else if (swapped_chr_fitness_b>=orig_chr_fitness && swapped_chr_fitness_b>=swapped_chr_fitness_a) {
        order_ = swap_b_order;
    }
}

ClimbChromosome::ClimbChromosome(const Cities *cities) : Chromosome(cities) {}
