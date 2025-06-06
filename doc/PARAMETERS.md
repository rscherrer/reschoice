## List of Parameters

Here we provide an explanation of the various parameters of the model (passed though a parameter file, e.g. `parameters.txt`), as well as their default value when no parameter file is supplied, and their valid ranges. For more information, please refer to the source code or the scientific manuscript accompanying this project (link will be made available once published).

Note that hereafter, "positive" means ***including zero***, while "strictly positive" means ***excluding zero***. Also note that missing parameters in the parameter file will take their default value.

| Parameter name | Default value(s) | Accepted values | Description | Notes |
|--|--|--|--|--|
| `popsize` | `10` | Strictly positive integers | Number of individuals in the population
| `xstart` | `0` | Decimals | Initial trait value of all individuals in the population
| `tradeoff` | `1` | Positive decimals | Strength of the utilization trade-off between the two resources | A value of 0 means no trade-off |
| `alpha` | `0` | Decimals from zero to one | Degree to which resource encounter is determined by their abundance | A value of 0 means that the probability of choosing a resource when choosing at random (`beta` is 0) is proportional to the abundance of that resource in the habitat, wherease a value of 1 means that the probability of choosing a resource at random is 0.5 |
| `beta` | `1` | Decimals from zero to one  | Degree of optimal choice | A value of 0 means that individuals choose randomly, while a value of 1 means that individuals always choose the most advantageous resource |
| `delta` | `1` | Positive decimals | Resource discovery rate | The per capita amount of resources discovered by an individual with unit resource consumption rate alone in its habitat (i.e. initial slope of the resource discovery function) |
| `hsymmetry` | `1` | Decimals from zero to one | Habitat symmetry | A value of 0 means that the two resources are completely asymmetrically distributed among the two habitats, while a value of 1 means that they are completely symmetrically distributed |
| `nrounds` | `10` | Strictly positive integers | Number of feeding rounds in a feeding season |
| `mutrate` | `0.0001` | Decimals from zero to one | Per individual mutation rate | The probability of any given offspring to mutate from its parent |
| `mutsdev` | `0.02` | Positive decimals | Mutational standard deviation | Standard deviation of the normal distribution from which mutations are sampled |
| `dispersal` | `0.01` | Decimals from zero to one | Per capita probability of any individual to disperse to the opposite habitat during the dispersal step | A value of 0 means no dispersal |
| `tend` | `100` | Strictly positive integers | Number of time steps in a simulation | The simulation will run for `tend` time steps, starting at time step 0 |
| `tsave` | `20` | Strictly positive integers | Frequency of data saving (in time steps) | The output will be saved every `tsave` time steps, including the first and last ones, for a total of **`tend` / `tsave` + 1** time steps saved |
| `seed` | Clock-generated | Positive integers | Seed of the pseudo-random number generator | The clock is used to generate a pseudo-random seed. Make sure to set `savepars` to 1 to be able to retrieve the generated seed and reproduce a given simulation. |
| `savepars` | `0` | One or zero | Whether or not to save the parameters into a parameter log file called `paramlog.txt` | If set to 1, the parameters will be saved in a file called `paramlog.txt` in the working directory |
| `savedat` | `0` | One or zero | Whether or not to save output data | If set to 1, the output data will be saved in the requested output file(s) in the working directory. Check [here](OUTPUT.md) for details on how to choose which variables to save. |
| `choose` | `0` | One or zero | Whether or not to choose which output variables to save by providing a `whattosave.txt` file | If set to 1, the program will read the `whattosave.txt` file in the working directory to determine which variables to save. See [here](OUTPUT.md) for how this works |
| `verbose` | `0` | One or zero | Whether or not to display progress at each time step to the screen | If set to 1, the program will display the current time step and the number of individuals in the population at each time step |
| `memsave` | `1` | Decimals greater than the size of a double precision floating point number (usually 8 bytes, i.e. 8e-06 MB, on a 64 bit system), in MB | Memory that can be filled in by each data saving buffer (in MB) before writing to file | This is the amount of memory that can be used by each output file before it is written to disk. There will be as many open buffers as there are output files to save (see details [here](OUTPUT.md)). |