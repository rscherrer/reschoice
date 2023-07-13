// This script contains all the functions of the Individual class

#include "individual.hpp"

// Constructor
Individual::Individual(const double &tradeoff) :
    x(0.0),
    eff1(exp(-tradeoff * utl::sqr(1 + x))),
    eff2(exp(-tradeoff * utl::sqr(1 - x))),
    alive(true)
{

}

// Function to kill an individual
void Individual::kill() { alive = false; }

// Getters
double Individual::getX() const { return x; }
double Individual::getEff1() const { return eff1; }
double Individual::getEff2() const { return eff2; }
bool Individual::isAlive() const { return alive; }