## Output Data

### Saving Parameters

If `savepars` is `1` the program saves the parameters it has used to run the simulation in a parameter log file called `paramlog.txt`. These include the `seed` of the pseudo random number generator and any parameters that have been updated (e.g. by reading them from file). See [here](PARAMETERS.md) for details about parameters.

### Screen Output

The program outputs to the screen messages about the status of the simulation at each generation (if `verbose` is `1`).

### Saving Data

The program saves data every `tsave` generations. The data are saved **in binary format**, in files with extension `.dat`. If data must be saved (`savedat 1`) and no variable choice is given (`choose 0`), by default the program saves all of the following:

| Name | Description | Frequency | 
|--|--|--|
| `time.dat` | Time step of each saved generation | 1 per time step |
| `resourceCensus.dat` | Number of individuals feeding on each resource at each feeding round in each habitat | 4 per feeding round |
| `resourceMeanTraitValue.dat` | Mean trait value of individuals feeding on each resource at each feeding round in each habitat | 4 per feeding round per time step |
| `individualExpectedFitnessDifference.dat` | Expected fitness difference between the resources for each individual at each feeding round | 1 per individual per feeding round |
| `individualChoice.dat` | Resource choice made by each individual at each feeding round | 1 per individual per feeding round |
| `individualRealizedFitness.dat` | Realized fitness gain of each individual after each feeding round | 1 per individual per feeding round |
| `individualRank.dat` | Position of each individual in the feeding queue at each feeding round | 1 per individual per feeding round |
| `individualHabitat.dat` | Habitat of each individual | 1 per individual |
| `individualTraitValue.dat` | Trait value of each individual | 1 per individual |
| `individualTotalFitness.dat` | Total fitness of each individual after all feeding rounds | 1 per individual |
| `individualEcotype.dat` | Ecotype of each individual (relative to population average) | 1 per individual |
| `habitatCensus.dat` | Number of individuals in each habitat | 2 per time step |
| `habitatMeanTraitValue.dat` | Mean trait value in each habitat | 2 per time step |
| `ecologicalIsolation.dat` | Ecological isolation statistic | 1 per time step |
| `spatialIsolation.dat` | Spatial isolation statistic | 1 per time step |

Note that the order of writing = habitat 1 resource 1, habitat 1 resource 1, habitat 2 resource 1, habitat 2 resource 2.

To save a subset of those output data files, set `choose 1` in the parameter file, and provide a list of variables to save in a file called `whattosave.txt`, located in the working directory. This could be, for example, a file containing:

```
time
resourceCensus
resourceMeanTraitValue
ecologicalIsolation
spatialIsolation
```

The names of the variables to save must be given **without the `.dat` extension**. 

For the sake of speed and modularity in combining outputs into tables, the data are converted to **double precision floating point numbers** before being saved as **binary**, and because the data is saved in this format, each file contains a **one-dimensional array** of data (e.g. `resourceCensus.dat` starts with the number of individuals feeding on resource 1 in habitat 1, followed by the number of individuals feeding on resource 2 in habitat 1, then the number of individuals feeding on resource 1 in habitat 2, and finally the number of individuals feeding on resource 2 in habitat 2, and this is repeated for each feeding round). The data are therefore not human-readable. To read them, you must convert them back into numbers using **a function decoding binary**, and knowledge of **how many bytes each value takes**. Although double precision-floating point numbers usually take up **8 bytes** on a 64-bit system, this might vary, and so we recommend to share the simulated data alongside platform details needed to read the data back in. To do just that and read the data into R, we developed the package [`readsim`](https://github.com/rscherrer/readsim).
