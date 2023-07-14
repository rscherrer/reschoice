// This script contains all the functions of the Individual class

#include "individual.hpp"

// Constructor
Individual::Individual(const double &tradeoff) :
    x(0.0),
    eff1(exp(-tradeoff * utl::sqr(1 + x))),
    eff2(exp(-tradeoff * utl::sqr(1 - x))),
    alive(true)
{

    // Check that feeding efficiencies are above zero
    assert(eff1 >= 0.0);
    assert(eff2 >= 0.0);

}

// Function to kill an individual
void Individual::kill() { alive = false; }

// Function to set the resource choice
void Individual::setChoice(const bool &r, const double &beta) { 
    
    // Probability of choosing optimally
    const double prob = 0.5 * (1.0 + beta);

    // Choose
    choice = rnd::bernoulli(prob)(rnd::rng) ? r : !r;
    
}



// Function to mutate an individual
void Individual::mutate(const double &dx, const double &tradeoff) {

    // Apply phenotypic deviation
    x += dx;

    // Update feeding efficiencies
    eff1 = exp(-tradeoff * utl::sqr(1 + x));
    eff2 = exp(-tradeoff * utl::sqr(1 - x));

}

// Getters
double Individual::getX() const { return x; }
double Individual::getEff1() const { return eff1; }
double Individual::getEff2() const { return eff2; }
bool Individual::getChoice() const { return choice; }
bool Individual::isAlive() const { return alive; }