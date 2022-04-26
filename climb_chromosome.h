/*
 * inherits from Chromosome class
 *
 * overrides mutate() and clone()
 */
#include "chromosome.h"

#pragma once

class ClimbChromosome : public Chromosome{

public:

    //ClimbChromosome(const Cities *cities_ptr) : Chromosome(cities_ptr) {}

    ClimbChromosome(const Cities *cities); //NOLINT


    void mutate() override;

    //ClimbChromosome* clone() const override {return new ClimbChromosome(cities_ptr_); }
    ClimbChromosome* clone() const override {return new ClimbChromosome(*this); } //NOLINT

};