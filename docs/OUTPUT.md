## Output data

Here is the list of variables that can be saved during a simulation:

Bit | File | Variable | No. values per time step |
|--|
| 1 | `time.dat` | time steps | 1 |
| 2 | `resourceCensus.dat` | number of individuals feeding on each resource at each feeding round in each habitat | 4 per feeding round* |
| 3 | `resourceMeanTraitValue.dat` | mean trait value of individuals feeding on each resource at each feeding round in each habitat | 4 per feeding round* |
| 4 | `individualExpectedFitnessDifference.dat` | expected fitness difference between the resources for each individual at each feeding round | 1 per individual per feeding round |
| 5 | `individualChoice.dat` | resource choice made by each individual at each feeding round | 1 per individual per feeding round |
| 6 | `individualRealizedFitness.dat` | realized fitness gain of each individual after each feeding round | 1 per individual per feeding round |
| 7 | `individualRank.dat` | position of each individual in the feeding queue at each feeding round | 1 per individual per feeding round |
| 8 | `individualHabitat.dat` | habitat of each individual | 1 per individual |
| 9 | `individualTraitValue.dat` | trait value of each individual | 1 per individual |
| 10 | `individualTotalFitness.dat` | total fitness of each individual after all feeding rounds | 1 per individual |
| 11 | `individualEcotype.dat` | ecotype of each individual (relative to population average) | 1 per individual |
| 12 | `habitatCensus.dat` | number of individuals in each habitat | 2 |
| 13 | `habitatMeanTraitValue.dat` | mean trait value in each habitat | 2 |
| 14 | `ecologicalIsolation.dat` | ecological isolation statistic | 1 |
| 15 | `spatialIsolation.dat` | spatial isolation statistic | 1 |

* Order of writing: habitat 1 resource 1, habitat 1 resource 1, habitat 2 resource 1, habitat 2 resource 2

### Encoding

The user can choose which variables to save, and must pass that information to the program via `whattosave` (in `parameters.txt`), a single positive integer whose binary representation is a series of zeros and ones indicating which of the above variables (each corresponding to a particular position, or bit, in the binary representation) must be saved (the ones). 

For example, `whattosave 1` has for binary representation:

15 14 13 12 11 10  9  8  7  6  5  4  3  2  1 (position)
 0  0  0  0  0  0  0  0  0  0  0  0  0  0  1 (value)

and so indicates the program that only `time.dat` must be saved (the bit corresponding to `time.dat` is the only one set to one).

Here is another example. If we want to save `time.dat`, `ecologicalIsolation.dat` and `spatialIsolation.dat`, the bit-string is:

15 14 13 12 11 10  9  8  7  6  5  4  3  2  1
 1  1  0  0  0  0  0  0  0  0  0  0  0  0  1

which is binary for the integer 24577. So, to save those three variables (and no other) we must specify `whattosave 24577` in `parameters.txt`. We recommend to use a binary calculator such as [this one](https://www.calculator.net/binary-calculator.html) to figure what parameter value to pass to the program.

