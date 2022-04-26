# HW 11 Travelling Salesperson Project (pt3)

`baseline-ga.tsv` is from pt2 of project.

## Class ClimbChromosome



- [x] Override `mutate()`: "local hill climbing" mutation, searches for best solution in a small local "neighborhood"
- [x] Override `clone()`
- [x] assert `is_valid` where prudent



- [x] modify `deme.cpp` to use ClimbChromosome instead of Chromosome
- [x] compile tsp
- [x] experiment w mutrate
- [x] save results into `local.tsv` to obtain baseline

Copied `local.tsv` from `cmake-build-debug/` to root.





## Class TournamentDeme 

inherits from deme class



* Tournament Selection: pits candidate parents against each other to det which parent gets to recombine

  * Select `P` parents at random (`P` is some const pwr of 2 that u choose, no bigger than pop size)

  * take first pair in the set of `P` parents and discard the parent w the lower fitness of the two
  * do same for next pair, etc
  * should have `P/2` resulting parents (assert this?)

* modify `tsp.cc` use TournamentDeme class instead of Deme

* experiment w size to get good results

* save results to "tournament.tsv"



## report results + extra credit



- [ ] Valgrind
- [ ] look over obsidian checkoff list 

