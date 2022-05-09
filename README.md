# HW 12 Multi-threading Time!

`threaded_randomized_search()` multi-threaded  wrapper for exhaustive search



`granular_randomized_search()` also multi-thread wrapper for exhaustive search, BUT

each thread runs `g` random searches without synchronizing, then tries to update the global best ordering (if improvement found). Play around with the value of `g` ('granularity') to see how at affects load balancing and overall run time.



- [x] Modify `ga_search()` in tsp to use the `nthread` parameter and run multiple demes in parallel, eventually returning the best solution across all demes. 

NUM_ITER = 4,000,000

mut_rate = 0.2

pop_size = 1000

Best Performer: 17003.9

### Output

1 thread

1514.15s user 6.81s system 99% cpu 25:24.27 total

2 thread

1550.33s user 3.61s system 198% cpu 13:01.31 total

4 thread

1585.59s user 3.66s system 396% cpu 6:40.70 total

8 thread

1614.08s user 3.65s system 757% cpu 3:33.46 total

16 thread

1842.53s user 4.66s system 962% cpu 3:11.83 total

![](Screen Shot 2022-05-08 at 22.57.23.png)





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

- [x] Copied `local.tsv` from `cmake-build-debug/` to root.





## Class TournamentDeme

inherits from deme class

* Tournament Selection: pits candidate parents against each other to det which parent gets to recombine

  * Select `P` parents at random (`P` is some const pwr of 2 that u choose, no bigger than pop size)

  * take first pair in the set of `P` parents and discard the parent w the lower fitness of the two
  * do same for next pair, etc
  * should have `P/2` resulting parents
  * repeat until 1 parent
* [x] modify `select_parent()` to do the above
* [x] modify `tsp.cc` use TournamentDeme class instead of Deme
* [x] experiment w size to get good results
* [x] test if `Chromosome` or `ClimbChromosome`gives better results
  *  `./HW11 challenge.tsv 100 0.2` ClimbChromosome (slightly) better
    *  ClimbChromosome: 18287.2, 17549.6, 18810.3
    *  Chromosome: 18393.4, 18463.1, 18264.8

* [x] save results to "tournament.tsv"
  *  `./HW11 challenge.tsv 1000 0.2 > tournament.tsv`

* [x] copy `tournament.tsv` to root

## Valgrind + Final Checks

`valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./HW11 challenge.tsv 100 0.2`

- [x] copy valgrind-out.txt to root

- [x] look over obsidian checkoff list 

## Reporting

Combined graph from “baseline-ga.tsv”, “local.tsv”, and “tournament.tsv”. 

Save the graph into “comparison.gif” 

`gnuplot`

`plot "baseline-ga.tsv" w lp, "local.tsv" w lp, "tournament.tsv" w lp`

`set term gif`

`set out "comparison.gif"`

`replot`



![](comparison.gif)

## Extra Credit

Alternating Edges Crossover (AEX)

![](comparison_aex.gif)

`valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out-aex.txt ./HW11`

`valgrind-out-aex.txt` attached.