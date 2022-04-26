/*
 * inherits from Chromosome class
 *
 * overrides mutate() and clone()
 */
#include "chromosome.h"

#pragma once

class ClimbChromosome : public Chromosome {

 public:

  explicit ClimbChromosome(const Cities *cities);

  void mutate() override;

  [[nodiscard]] ClimbChromosome *clone() const override { return new ClimbChromosome(*this); }

};