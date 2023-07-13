#ifndef RESCHOICE_RANDOM_HPP
#define RESCHOICE_RANDOM_HPP

// This header contains aliases for various kinds of probability distributions
// as well as a random number generator. Include this header in other scripts
// to be able to use it.

// Example usage:
// double x = rnd::normal(0.0, 1.0)(rnd::rng);
// to sample a number from a normal distribution with mean zero and standard
// deviation one. Alternatively, use:
// auto mynormal = rnd::normal(0.0, 1.0);
// to set up the normal distribution and only sample from it when needed:
// double x = mynormal(rnd::rng);
// (This can be useful when sampling many times, as setting up a distribution
// takes time...)

#include <random>

namespace rnd
{

    // Probability distributions
    typedef std::discrete_distribution<size_t> discrete;
    typedef std::uniform_int_distribution<size_t> random;
    typedef std::exponential_distribution<double> exponential;
    typedef std::binomial_distribution<size_t> binomial;
    typedef std::poisson_distribution<size_t> poisson;
    typedef std::geometric_distribution<size_t> geometric;
    typedef std::uniform_real_distribution<double> uniform;
    typedef std::normal_distribution<double> normal;
    typedef std::gamma_distribution<double> gamma;
    typedef std::bernoulli_distribution bernoulli;

    // Random number generator
    extern std::mt19937_64 rng;

}

#endif
