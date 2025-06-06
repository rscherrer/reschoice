#ifndef RESCHOICE_PARAMETERS_HPP
#define RESCHOICE_PARAMETERS_HPP

// Parameter set. Contains values of the parameters of the simulation.
// All parameters have default values that can be modified by calling
// the program with a parameter file name as unique argument.

#include "random.hpp"
#include "reader.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cassert>

struct Parameters {

    // Constructor
    Parameters(const std::string& = "");

    // Functions
    void check() const;
    void read(const std::string&);
    void save(const std::string&) const;

    // Parameters
    size_t popsize;      // fixed population size
    double xstart;       // starting trait value
    double tradeoff;     // resouce utilization tradeoff    
    double alpha;        // resource abundance weight
    double beta;         // resource choice accuracy
    double delta;        // resource discovery rate
    double hsymmetry;    // habitat symmetry parameter
	size_t nrounds;      // number of feeding rounds
	double mutrate;      // mutation rate
	double mutsdev;      // mutational standard deviation
    double dispersal;    // dispersal rate
    size_t tend;         // simulation time
    size_t tsave;        // recording time
    size_t seed;         // seed for the random number generator
    bool savepars;       // whether to save parameters into a file
    bool savedat;        // whether to save output data into files
    bool verbose;        // whether to output to screen
    bool choose;         // whether to choose the variables to save
    double memsave;      // memory used for data storage (in MB)

};

#endif