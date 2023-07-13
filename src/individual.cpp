// This script contains all the functions of the Individual class

#include "individual.hpp"

// Constructor
Individual::Individual() :
    x(0.0),
    alive(true)
{

}

// Function to kill an individual
void Individual::kill() { alive = false; }

// Is an individual alive?
bool Individual::isAlive() const { return alive; }