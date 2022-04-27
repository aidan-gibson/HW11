//
// Created by Aidan Gibson on 4/26/2022.
//
#include <cassert> //for linux compat
#include <algorithm>
#include <chrono> //for linux compat
#include "chromosome.h"
#include "AltChromosome.h"

using namespace std;


pair<Chromosome *, Chromosome *> AltChromosome::recombine(const Chromosome *other) {
  assert(is_valid());
  assert(other->is_valid());



  //implementing alternating edges crossover
  auto child1 = create_ae_crossover_child(this, other);
  auto child2 = create_ae_crossover_child(other, this);


  assert(is_valid());
  assert(other->is_valid());
  assert(order_.size()>0);
  return std::make_pair(child1, child2);
}
bool elem_in_vec(vector<unsigned> vec, unsigned value){
  if (find(vec.begin(), vec.end(),value)!=vec.end()) {
    return true;
  }
  return false;
}
Chromosome *AltChromosome::create_ae_crossover_child(const Chromosome *p1, const Chromosome *p2) {
  const unsigned len = p1->order_.size();
  assert(len == p2->order_.size());
  Chromosome* child = p1->clone();
  //child->order_.clear();
  //child->order_.reserve(p2->order_.size());
  Chromosome* temp = p1->clone();
  auto unused_arr = temp->order_;

  //child->order_[0] = p1->order_[0]; //hard coding the init copying of the first 2 elem
  //child->order_[1] = p1->order_[1]; //clone() take care of this for me.redundant
  unused_arr.erase(unused_arr.begin());
  unused_arr.erase(unused_arr.begin()); //pops unused_arr[0] & temp_arr[1]
  unsigned index = 1;

  //while (index<child->order_.size()) {
  while (unused_arr.size()>0) {

    //where is child->order_[index] in p2? we want NEXT one.
    auto next_arc = find(p2->order_.begin(),
                         p2->order_.end(),
                         child->order_[index]); //iterator of where child->order_[1] is in p2->order_

    //but what if it's the last elem in p2? we want to stay in bounds! we also want it to be an int, not an iterator

    auto next_arc_index = (next_arc - (p2->order_.begin()) + 1)
        % (p2->order_.size()); //find uses iterators, so we subtract by order_begin() to go back to an int, and we add 1 to go to the next value

    //what if p2->order_[next_arc_index] is already part of child->order_???? pick random unused vertex
//    while (elem_in_vec(child->order_, p2->order_[next_arc_index])) { //finding random unused vertex
//      //if it's already part of child->order_, pick random unused vertex
//      generator_.seed(chrono::system_clock::now().time_since_epoch().count());
//      next_arc_index = generator_() % p2->order_.size();//random elem which isn't already in child->order_ (hopefully)
//    }

    if (elem_in_vec(child->order_,p2->order_[next_arc_index])){
      //pick rando element from unused_arr, remove it from unused arr
      generator_.seed(chrono::system_clock::now().time_since_epoch().count());
      auto temp_index = generator_() % unused_arr.size();
      child->order_[++index] = unused_arr[temp_index];
      unused_arr.erase(temp_index+unused_arr.begin());
    }
    else {
      unused_arr.erase(find(unused_arr.begin(),
                            unused_arr.end(),
                            p2->order_[next_arc_index])); //erases p2->order_[next_arc_index] value from unused_arr
      child->order_[++index] = p2->order_[next_arc_index];
    }
    //arc back to p1
     next_arc = find(p1->order_.begin(),
                         p1->order_.end(),
                         child->order_[index]); //iterator of where child->order_[1] is in p2->order_

    //but what if it's the last elem in p2? we want to stay in bounds! we also want it to be an int, not an iterator

     next_arc_index = (next_arc - (p1->order_.begin()) + 1)
        % (p1->order_.size()); //find uses iterators, so we subtract by order_begin() to go back to an int, and we add 1 to go to the next value

    //what if p2->order_[next_arc_index] is already part of child->order_???? pick random unused vertex
//    while (elem_in_vec(child->order_, p2->order_[next_arc_index])) { //finding random unused vertex
//      //if it's already part of child->order_, pick random unused vertex
//      generator_.seed(chrono::system_clock::now().time_since_epoch().count());
//      next_arc_index = generator_() % p2->order_.size();//random elem which isn't already in child->order_ (hopefully)
//    }

    if (elem_in_vec(child->order_,p1->order_[next_arc_index])){
      //pick rando element from unused_arr, remove it from unused arr
      generator_.seed(chrono::system_clock::now().time_since_epoch().count());
      auto temp_index = generator_() % unused_arr.size();
      child->order_[++index] = unused_arr[temp_index];
      unused_arr.erase(temp_index+unused_arr.begin());
    }
    else {
      unused_arr.erase(find(unused_arr.begin(),
                            unused_arr.end(),
                            p1->order_[next_arc_index])); //erases p2->order_[next_arc_index] value from unused_arr
      child->order_[++index] = p1->order_[next_arc_index];
    }
  }
//there wasn't a 23 and there were 2x 37's. there's smth at beginning or end of loop messing shit up
  assert(child->is_valid());
  return child;
}
AltChromosome::AltChromosome(const Cities *cities)
    : Chromosome(cities), ClimbChromosome(cities) {}

