/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>
#include <chrono> //for linux compat

#include "chromosome.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities *cities_ptr)
        : cities_ptr_(cities_ptr),
          order_(random_permutation(cities_ptr->size())),
          generator_(rand()) {
    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome() {
    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
/*
 * Perform a single mutation on this chromosome
 * pick two points at random and swap their values!
 */
void Chromosome::mutate() {
    generator_.seed(chrono::system_clock::now().time_since_epoch().count()); //using current time as seed
    unsigned int swap_pos_a = generator_() % order_.size();
    unsigned int swap_pos_b = generator_() % order_.size();

    while (swap_pos_a ==
           swap_pos_b) { // ensures they are diff values; if both a and b randomly get 0, no swap will occur
        swap_pos_b = generator_() % order_.size();
    }
    swap(order_[swap_pos_a], order_[swap_pos_b]);
    assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
//Return a pair of offsprings by recombining with another chromosome
//Note: this method allocates memory for the new offsprings
//ordered crossover, where a random consecutive subset is copied from one parent to the child, and the rest of the cities are copied from the other parent in the //original order
pair<Chromosome *, Chromosome *> Chromosome::recombine(const Chromosome *other) {
    assert(is_valid());
    assert(other->is_valid());

    generator_.seed(chrono::system_clock::now().time_since_epoch().count()); //using current time as seed
    unsigned int a = generator_() % order_.size();
    unsigned int b = generator_() % order_.size();
    while (a == b) { // ensures they are diff values
        b = generator_() % order_.size();
    }
    auto child_a = create_crossover_child(this, other, min(a, b), max(a, b));

    unsigned int c = generator_() % order_.size();
    unsigned int d = generator_() % order_.size();
    while (c == d) { // ensures they are diff values
        d = generator_() % order_.size();
    }
    auto child_b = create_crossover_child(other, this, min(c, d), max(c, d));

    return {child_a, child_b};

}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome *
Chromosome::create_crossover_child(const Chromosome *p1, const Chromosome *p2,
                                   unsigned b, unsigned e) const {
    Chromosome *child = p1->clone();

    // We iterate over both parents separately, copying from parent1 if the
    // value is within [b,e) and from parent2 otherwise
    unsigned i = 0, j = 0;

    for (; i < p1->order_.size() && j < p2->order_.size(); ++i) {
        if (i >= b and i < e) {
            child->order_[i] = p1->order_[i];
        } else { // Increment j as long as its value is in the [b,e) range of p1
            while (p1->is_in_range(p2->order_[j], b, e)) {
                ++j;
                assert(j < p2->order_.size());
            }
            child->order_[i] = p2->order_[j];
            j++;
        }
    }

    assert(child->is_valid());
    return child;
}


/*
 * Return a positive fitness value, with higher numbers representing fitter solutions (shorter total-city traversal path).
 */
double Chromosome::get_fitness() const {
    return 1.0 / calculate_total_distance();
}

/*
 * A chromosome is valid if it has no repeated values in its permutation, as well as no indices above the range (length) of the chromosome.
 */
bool Chromosome::is_valid() const {
    // order_ is permutation_t, like {0,1,2,3,4} (std::vector<unsigned int>)
    // no repeated values.
    // within range

    //(copy the vector) then sort it (u dont wanna modify the orig perm)
    Cities::permutation_t sorted_order_;
    copy(order_.begin(), order_.end(), back_inserter(sorted_order_)); //creates a sorted copy of order_ sorted_order_
    sort(sorted_order_.begin(), sorted_order_.end());
    //check if they're all in range by checking top and bottom of sorted
    if (sorted_order_.front() != 0) {
        return false; //false if the first elem isn't 0
    }
    if (sorted_order_.back() != (sorted_order_.size() - 1)) {
        return false; //false if last elem isn't size-1
    }
    //check for duplicates ez cuz sorted
    for (unsigned int i = 0; i < sorted_order_.size() - 1; i++) { //checking if i is same as
        for (unsigned int j = i + 1; j < sorted_order_.size(); j++) { //every next one on the list
            if (i == j) {
                return false;
            }
        }
    }
    return true;
}

/* Find whether a certain value appears in a given range of the chromosome.
 * Returns true if value is within the specified the range specified
 * [begin, end) and false otherwise.
 */
bool Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const //NOLINT
{
    //making clang-tidy shut up about making this function static; per instructions Eitan hasn't allowed us to modify what was given in header file.

    for (unsigned int i = begin; i < end; i++) {
        if (value == order_[i]) {
            return true;
        }
    }
    return false;
}