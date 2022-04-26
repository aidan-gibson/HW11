/*
 * inherits from Deme class
 *
 * overrides whatever is necessary in your judgement for your class to replace the parent selection algorithm.
 * (It can use either Chromosome or ClimbChromosome: experiment with both when you're done and pick the best one.)
 *
 */
#include "deme.h"

#pragma once

class TournamentDeme : public Deme{
 public:

  explicit TournamentDeme(const Cities *cities_ptr, unsigned pop_size, double mut_rate);

  Chromosome * select_parent() override;
};