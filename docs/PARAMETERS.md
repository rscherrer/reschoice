## List of parameters

| Name | Meaning | Default | Note |
|--|--|--|--|
| popsize | Population size | 1000 | The population size is fixed through time |
| xstart | Initial trait value | -1 | The trait value of all individuals at the start of the simulation |
| tradeoff | Trade-off strength | 1 | The strength of the utilization trade-off between the two resources |
| beta | Degree of optimal choice | 0 | The probability with which individuals choose the most advantageous resource |
| delta | Resource discovery rate | 1 | A parameter scaling the degree of soft versus hard selection |
| resource | Quantity of resource | 100 | Both resources, in arbitrary units (replenished after each feeding round) |
| hsymmetry | Habitat symmetry | 1 | The degree of symmetry in resource distribution between the two habitats |
| nrounds | Number of feeding rounds | 10 | Number of feeding rounds in a feeding season |
| typeII | Type II resource choice (0/1) | 0 | Whether type II resource choice should be used instead of type I |
| mutrate | Mutation rate | 0.0001 | Per individual |
| mutsdev | Mutational standard deviation | 0.02 | Standard deviation of the normal distribution mutations are sampled from |
| dispersal | Dispersal rate | 0.1 | Chances that an individual disperses to the opposite habitat |
| tend | Simulation time | 1000 | Number of time steps in a simulation |
| tsave | Saving frequency | 10 | Save output every how many time steps? |
| seed | Seed for the random number generator | Determined by the clock | |
| savepars | Whether to save parameters (0/1) | 1 | If yes, saved to `paramlog.txt` |
| savelog | Whether to save screen output (0/1) | 0 | If yes, saved to `log.txt` |
| talkative | Whether to output time steps to screen (0/1) | 1 | |
| whattosave | Which variable(s) to save as output | 1 | Click [here](OUTPUT.md) for details |
