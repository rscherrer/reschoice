#ifndef RESCHOICE_PARAMETERS_HPP
#define RESCHOICE_PARAMETERS_HPP

// Parameter set. Contains values of the parameters of the simulation.
// All parameters have default values that can be modified by calling
// the program with a parameter file name as unique argument.

#include "random.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cassert>

struct Parameters {

    Parameters();

    void read(const std::string&);
    void update();
    void import(std::ifstream&);
    void write(std::ofstream&) const;
    void save() const;
    void check() const;
    size_t makeDefaultSeed();

    // Parameters
    size_t popsize; // fixed population size
	double tradeoff; // resouce utilization tradeoff
	double beta; // resource choice accuracy
	double delta; // resource discovery rate
	double resource; // baseline amount of each resource
    double hsymmetry; // habitat symmetry parameter
	size_t nrounds; // number of feeding rounds
	double mutrate; // mutation rate
	double mutsdev; // mutational standard deviation
    double dispersal; // dispersal rate
    size_t tend; // simulation time
    size_t tsave; // recording time
    size_t seed; // seed for the random number generator
    bool savepars; // whether to save parameters into a file
    bool savelog; // whether to redirect screen output to a log file
    bool talkative; // whether to output to screen
    bool choose; // whether to choose which variables to save

};

#endif