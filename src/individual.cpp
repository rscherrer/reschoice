// This script contains all the functions of the Individual class

#include "individual.hpp"

// Spontaneous generation constructor
Individual::Individual(const double &xval, const double &tradeoff) :
    x(xval),
    eff1(0.0),
    eff2(0.0),
    diff(0.0),
    habitat(false),
    ecotype(false),
    choice(false),
    rank(0u)
{

    // Update feeding efficiencies
    develop(tradeoff);

}

// Function to calculate baseline probability
double ind::probbase(const double &rbest, const double &rboth, const double &alpha) {

    // rbest: amount of the best resource
    // rboth: sum of both resources
    // alpha: resource abundance weight

    // Check
    assert(rbest >= 0.0);
    assert(rboth >= rbest);
    assert(rboth > 0.0);
    assert(alpha >= 0.0);
    assert(alpha <= 1.0);

    // Compute baseline probability
    return 0.5 * (1.0 - alpha) + alpha * rbest / rboth;

}

// Function to calculate the probability of choosing the best resource
double ind::probbest(const double &pbase, const double &beta) {

    // pbase: baseline probability in the absence of choice
    // beta: resource choice parameter

    // Check
    assert(pbase >= 0.0);
    assert(pbase <= 1.0);
    assert(beta >= 0.0);
    assert(beta <= 1.0);

    // Compute the probability
    return (1.0 - beta) * pbase + beta;

}

// Function to set the resource choice
void Individual::makeChoice(const double &fit1, const double &fit2, const double &beta, const double &alpha, const double &res1, const double &res2) { 

    // fit1, fit2: expected payoff for each resource
    // beta: optimal choice parameter
    // alpha: weight of resource abundance when choice is random
    // res1, res2: total contrentrations of both resources

    // Concentration of the best resource
    const double rbest = fit2 > fit1 ? res2 : res1;

    // Baseline probability of choosing the best resource without choice
    const double pbase = ind::probbase(rbest, res1 + res2, alpha);

    // Check
    assert(pbase >= 0.0);
    assert(pbase <= 1.0);

    // Probability of choosing the best resource
    const double prob = ind::probbest(pbase, beta);

    // Check
    assert(prob >= 0.0);
    assert(prob <= 1.0);

    // Is the best resource chosen?
    const bool isAccurate = rnd::bernoulli(prob)(rnd::rng);

    // Make the choice
    choice = (fit2 > fit1) == isAccurate;
    
}

// Function to record the expected fitness difference
void Individual::setDiff(const double &value) {

    diff = value;

}

// Function to mutate an individual
void Individual::mutate(const double &dx, const double &tradeoff) {

    // Apply phenotypic deviation
    x += dx;

    // Update feeding efficiencies
    develop(tradeoff);

}

// Function to set the ecotype of an individual
void Individual::setEcotype(const double &meanx) {

    // Is the trait value above or below population average?
    ecotype = x > meanx;

}

// Function to update feeding efficiencies
void Individual::develop(const double &tradeoff) {

    // Update feeding efficiencies
    eff1 = exp(-tradeoff * utl::sqr(x + 1.0));
    eff2 = exp(-tradeoff * utl::sqr(x - 1.0));

    // Check that feeding efficiencies are still above zero
    assert(eff1 >= 0.0);
    assert(eff2 >= 0.0);

}

// Function to switch the habitat of an individual to the opposite
void Individual::disperse() {

    habitat = !habitat;

}

// Function to assign a rank in the queue to an individual during a feeding round
void Individual::setRank(const size_t &i) {

    rank = i;

}

// Getters
double Individual::getX() const { return x; }
double Individual::getEff1() const { return eff1; }
double Individual::getEff2() const { return eff2; }
double Individual::getDiff() const { return diff; }
bool Individual::getHabitat() const { return habitat; }
bool Individual::getEcotype() const { return ecotype; }
bool Individual::getChoice() const { return choice; }
size_t Individual::getRank() const { return rank; }