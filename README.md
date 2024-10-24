# reschoice

This is the source code for the program `reschoice`, an individual-based simulation of evolutionary branching under various degrees of resource choice.

## Model description

This is a simulation of the evolution of a population of individuals through time, where what evolves is the trait values of individuals. Those trait values allow individuals to feed more-or-less efficiently on two types of resources spread across two habitats, with a utilization trade-off between the two resources. Under some parameter values those conditions should promote evolutionary branching into two separate clusters of individuals, each better adapted at feeding on one of the two resources. Notably, every generation, individuals go through multiple feeding rounds during which they make decisions on which resource to feed on, and those decisions are informed by the expected payoff each resource may bring to the individual. See manuscript for more details.

## Build

You can use the CMake files provided here to build the program, for example by using this [setup](https://github.com/rscherrer/setupp).

## Usage

This program is a command-line interface program. Assuming the name of the executable is `reschoice`, use the following command in the terminal from the working directory to run a simulation with default parameter values:

```shell
./reschoice
```

To run a simulation with custom parameters, use:

```shell
./reschoice parameters.txt
```

where `parameters.txt` is a text file containing a list of parameter names and values.

### Parameters

The file `parameters.txt` should look like:

```
popsize 1000
xstart -1
tradeoff 1
alpha 0
beta 1
delta 1
hsymmetry 1
nrounds 10
mutrate 0.0001
mutsdev 0.02
dispersal 0.1
tend 1000
tsave 10
seed 1689855017669157166
savepars 1
savelog 0
talkative 1
whattosave 1
```

and should be present within the working directory where the simulation is launched. Click [here](doc/PARAMETERS.md) for a detailed description of the parameters and their meaning.

### Output

The user can choose which variables to save from the simulation. Those have to be encoded in the `whattosave` parameter within the `parameters.txt` file, as explained [here](doc/OUTPUT.md). Output data are saved into binary files with the `.dat` extension, which can be read by the custom-made [R](https://www.r-project.org/) package [reschoicer](https://github.com/rscherrer/reschoicer).

## About

This program is a descendent of a [program](https://github.com/J-Damhuis/Eco-Evolutionary-Dynamics) originally written by [Joris Damhuis](https://github.com/J-Damhuis/). This code comes with no guarantee whatsoever.

## Links

* [reschoicer](https://github.com/rscherrer/reschoicer): package to read the simulations in R
* [reschoice-data](https://github.com/rscherrer/reschoice-data): the simulated data
* [reschoice-results](https://github.com/rscherrer/reschoice-results): scripts used to analyze the data and results of the study
* [reschoice-ms](https://github.com/rscherrer/reschoice-ms): manuscript
* [reschoice-nb](https://github.com/rscherrer/reschoice-nb): some derivations in Mathematica

## Permissions

Copyright (c) Raphaël Scherrer, 2023 (open source license will be added upon publication).
