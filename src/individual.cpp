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
void Individual::makeChoice(const double &fit1, const double &fit2, const double &beta) { 
    
    // Calculate the noise around fitness assessment
    const double noise = fabs(fit2 - fit1) * (1.0 + sqrt(1.0 - beta)) / beta;

    // Sample estimated fitness gains given uncertainty
    const double fit1_ = rnd::uniform(fit1 - 0.5 * noise, fit1 + 0.5 * noise)(rnd::rng);
    const double fit2_ = rnd::uniform(fit2 - 0.5 * noise, fit2 + 0.5 * noise)(rnd::rng);

    // Choose the resource that gives the best estimated payoff
    choice = fit2_ > fit1_;
    
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