# HW 12 Multi-threading Time!

`threaded_randomized_search()` multi-threaded  wrapper for exhaustive search



`granular_randomized_search()` also multi-thread wrapper for exhaustive search, BUT

each thread runs `g` random searches without synchronizing, then tries to update the global best ordering (if improvement found). Play around with the value of `g` ('granularity') to see how at affects load balancing and overall run time.



- [ ] Modify `ga_search()` in tsp to use the `nthread` parameter and run multiple demes in parallel, eventually returning the best solution across all demes. 

instead of patty i can use any computer with 4+ cores (my laptop). gotta make sure the linux VM i use has multiple core access.

Adapting this code to run a parallel GA should not prove difficult, but debugging it may be. Make sure the results make sense, there are no spurious crashes or errors, and it passes valgrind / address sanitizer. Then, you need to ensure that performance is adequate. To this end, run your code on `patty.reed.edu`, which is a 32-core/64-thread Linux computer. Measure your program with 1, 2, 4, 8, 16, 32, and 64 threads, and plot the total runtime against no. of threads as a graph (using gnuplot / Excel / whatever). You can measure the total runtime of the program as you did in HW5.

Submit this graph in your assignment, as well as a short README that tries to explain the observed performance in the graph. To receive full credit for your assignment, your best performance point in this graph needs to be at least 10x faster than the single-threaded performance (which in turn needs to be similar to the performance you got in part 3, HW11).

When you're done, upload your modified code, along with `shortest.csv` and a README with any special compilation instructions AND the following information:

- The performance graph.
- Which parameters did you choose: num_iter, pop_size, mut_rate, and `nthread` for the best performer.
- How long is the resulting best tour you found (distance).
- What is the run time, and how does it compare to a single-threaded run time (use the same parameters to compare, except `nthread=1`).

As a point of reference, my solution searches 9 million individuals (`NUM_ITER` = 9000000) in about 5s on my old home computer, using the somewhat arbitrary parameters `pop_size` = 1000, `mut_rate` = 0.3 and `nthreads`=14 (my machine has 28 cores). In one of those runs, it came up with what appears to be a close-to-optimal solution (total distance = 5477). Can your GA beat this?

If you want to make your GA work even better, you can synchronize the threads at points of your choosing (say, every G generations) and exchange some "best chromosomes" among them, to spruce up their gene pool. This improvement isn't required for full credit, however.



- [ ] Submit to github [here](https://moodle.reed.edu/mod/assign/view.php?id=199135) specify branch









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