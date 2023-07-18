#ifndef RESCHOICE_SIMULATION_HPP
#define RESCHOICE_SIMULATION_HPP

// This is the header containing the main simulation function

#include "parameters.hpp"
#include "individual.hpp"
#include "buffer.hpp"
#include "random.hpp"
#include "printer.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

// Main simulation function
int simulate(const std::vector<std::string>&);

#endif
