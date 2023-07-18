// This script contains all the functions of the Individual class

#include "individual.hpp"

// Spontaneous generation constructor
Individual::Individual(const double &tradeoff) :
    x(0.0),
    eff1(0.0),
    eff2(0.0),
    habitat(false),
    ecotype(false),
    choice(false),
    index(0u),
    alive(true)
{

    // Update feeding efficiencies
    develop(tradeoff);

}

// Function to kill an individual
void Individual::kill() { alive = false; }

// Function to make sure an individual is alive
void Individual::isBorn() { alive = true; }

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
    develop(tradeoff);

}

// Function to update feeding efficiencies
void Individual::develop(const double &tradeoff) {

    // Update feeding efficiencies
    eff1 = exp(-tradeoff * utl::sqr(1 + x));
    eff2 = exp(-tradeoff * utl::sqr(1 - x));

    // Check that feeding efficiencies are still above zero
    assert(eff1 >= 0.0);
    assert(eff2 >= 0.0);

}

// Getters
double Individual::getX() const { return x; }
double Individual::getEff1() const { return eff1; }
double Individual::getEff2() const { return eff2; }
bool Individual::getHabitat() const { return habitat; }
bool Individual::getEcotype() const { return ecotype; }
bool Individual::getChoice() const { return choice; }
size_t Individual::getIndex() const { return index; }
bool Individual::isAlive() const { return alive; }