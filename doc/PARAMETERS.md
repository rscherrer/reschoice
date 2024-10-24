## List of parameters

| Name | Meaning | Default | Note |
|--|--|--|--|
| popsize | Population size | 10 | The population size is fixed through time |
| xstart | Initial trait value | 0 | The trait value of all individuals at the start of the simulation |
| tradeoff | Trade-off strength | 1 | The strength of the utilization trade-off between the two resources |
| alpha | Resource abundance weight | 0 | Degree to which resource encounter is determined by their abundance |
| beta | Degree of optimal choice | 1 | The probability with which individuals choose the most advantageous resource |
| delta | Resource discovery rate | 1 | A parameter scaling the degree of soft versus hard selection |
| hsymmetry | Habitat symmetry | 1 | The degree of symmetry in resource distribution between the two habitats |
| nrounds | Number of feeding rounds | 10 | Number of feeding rounds in a feeding season |
| mutrate | Mutation rate | 0.0001 | Per individual |
| mutsdev | Mutational standard deviation | 0.02 | Standard deviation of the normal distribution mutations are sampled from |
| dispersal | Dispersal rate | 0.1 | Chances that an individual disperses to the opposite habitat |
| tend | Simulation time | 100 | Number of time steps in a simulation |
| tsave | Saving frequency | 20 | Save output every how many time steps? |
| seed | Seed for the random number generator | Determined by the clock | |
| savepars | Whether to save parameters (0/1) | 1 | If yes, saved to `paramlog.txt` |
| savelog | Whether to save screen output (0/1) | 0 | If yes, saved to `screenlog.txt` |
| talkative | Whether to output time steps to screen (0/1) | 0 | |
| whattosave | Which variable(s) to save as output | 1 | Click [here](OUTPUT.md) for details |
