// This script contains function definitions for the als namespace

#include "analysis.hpp"

// Function to compute the ecological isolation statistic
double als::getEI(
	
	const std::vector<std::vector<size_t> > &n,
	const std::vector<std::vector<double> > &sumx,
	const std::vector<std::vector<double> > &ssqx
	
) {

	// Compute within-ecotype and whole-population variances
	const size_t n1 = n[0u][0u] + n[1u][0u];
	const size_t n2 = n[0u][1u] + n[1u][1u];
	const size_t n0 = n1 + n2;
	const double sumx1 = sumx[0u][0u] + sumx[1u][0u];
	const double sumx2 = sumx[0u][1u] + sumx[1u][1u];
	const double ssqx1 = ssqx[0u][0u] + ssqx[1u][0u];
	const double ssqx2 = ssqx[0u][1u] + ssqx[1u][1u];
	const double varx1 = n1 ? ssqx1 / n1 - utl::sqr(sumx1 / n1) : 0.0;
	const double varx2 = n2 ? ssqx2 / n2 - utl::sqr(sumx2 / n2) : 0.0;
	const double varx0 = (ssqx1 + ssqx2) / n0 - utl::sqr((sumx1 + sumx2) / n0);

	// Make sure the variances are positive
	assert(varx1 >= 0.0);
	assert(varx2 >= 0.0);
	assert(varx0 >= 0.0);

	// Compute ecological isolation between ecotypes
	const double EI = varx0 ? 1.0 - (n1 * varx1 + n2 * varx2) / (n0 * varx0) : 0.0;

	// Make sure ecological isolation is between zero and one
	assert(EI >= 0.0 && EI <= 1.0);

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
	const double SI = prod ? fabs((n11 * n22 - n12 * n21) / sqrt(prod)) : 0.0;

	// Make sure spatial isolation is between zero and one
	assert(SI >= 0.0 && SI <= 1.0);

	return SI;

}

