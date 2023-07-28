// This script contains function definitions for the als namespace

#include "analysis.hpp"

// Function to compute the ecological isolation statistic
double als::getEI(
	
	const std::vector<std::vector<size_t> > &n,
	const std::vector<std::vector<double> > &sumx,
	const std::vector<std::vector<double> > &ssqx
	
) {

	// Comput the variance in trait values in the population
	const size_t n0 = n[0u][0u] + n[0u][1u] + n[1u][0u] + n[1u][1u];
	const double sumx0 = sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]; 
	const double ssqx0 = ssqx[0u][0u] + ssqx[0u][1u] + ssqx[1u][0u] + ssqx[1u][1u];
	double varx0 = ssqx0 / n0 - utl::sqr(sumx0 / n0);

	// Correct small numerical imprecisions
	varx0 = varx0 < 0.0 && varx0 > -utl::precis() ? 0.0 : varx0;

	// Make sure the variance is positive
	assert(varx0 >= 0.0);

	// Compute standard deviation in trait values as our metric for ecological isolation
	const double EI = sqrt(varx0);

	return EI;

}

// Function to compute the spatial isolation statistic
double als::getSI(const std::vector<std::vector<size_t> > &n) {

    // Different components of the statistic
	const size_t n11 = n[0u][0u];
	const size_t n12 = n[0u][1u];
	const size_t n21 = n[1u][0u];
	const size_t n22 = n[1u][1u];
	const size_t n10 = n11 + n12;
	const size_t n20 = n21 + n22;
	const size_t n01 = n11 + n21;
	const size_t n02 = n12 + n22;

	// Compute the product that will go in the denominator
	const double prod = n10 * n20 * n01 * n02;

	// Make sure it is positive
	assert(prod >= 0.0);

	// Compute spatial isolation
	const double a = n11 * n22;
	const double b = n12 * n21;
	const double SI = prod ? fabs((a - b) / sqrt(prod)) : 0.0;

	// Note: somehow huge numerical imprecision errors if I don't compute a and b separately

	// Make sure spatial isolation is between zero and one
	assert(SI >= 0.0 && SI <= 1.0);

	return SI;

}

