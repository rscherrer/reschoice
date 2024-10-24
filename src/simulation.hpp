#ifndef RESCHOICE_SIMULATION_HPP
#define RESCHOICE_SIMULATION_HPP

// This is the header containing the main simulation function

#include "parameters.hpp"
#include "individual.hpp"
#include "random.hpp"
#include "printer.hpp"
#include "analysis.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

// Resource discovery function
double calcResourceDiscovered(const double&, const double&, const double&);

// Fitness function
double calcFitness(const double&, const double&, const double&, const size_t& = 1u);

// Main simulation function
int simulate(const std::vector<std::string>&);

#endif
