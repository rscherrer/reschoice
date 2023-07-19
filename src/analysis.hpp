#ifndef RESCHOICE_ANALYSIS_HPP
#define RESCHOICE_ANALYSIS_HPP

// This is the header for the als namespace

#include "utilities.hpp"
#include <vector>
#include <cmath>
#include <cassert>

namespace als
{

	double getEI(const std::vector<std::vector<size_t> >&, const std::vector<std::vector<double> >&, const std::vector<std::vector<double> >&);
	double getSI(const std::vector<std::vector<size_t> >&);

}

#endif
