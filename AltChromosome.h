//
// Created by Aidan Gibson on 4/26/2022.
//
#include "climb_chromosome.h"

class AltChromosome : virtual public Chromosome, virtual public ClimbChromosome {
 public:

  AltChromosome(const Cities *cities);
  ~AltChromosome() override;
  std::pair<Chromosome *, Chromosome *> recombine(const Chromosome *other) override;

  Chromosome *create_ae_crossover_child(const Chromosome *p1, const Chromosome *p2);
};