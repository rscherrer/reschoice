// This script contains all the functions of the Individual class

#include "individual.hpp"

// Spontaneous generation constructor
Individual::Individual(const double &tradeoff) :
    x(0.0),
    eff1(0.0),
    eff2(0.0),
    diff(0.0),
    habitat(false),
    ecotype(false),
    choice(false),
    rank(0u),
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
bool Individual::isAlive() const { return alive; }