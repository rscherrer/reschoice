#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

#include "utilities.hpp"
#include <cmath>

class Individual {

public:

    Individual(const double&);

    void kill();

    double getX() const;
    double getEff1() const;
    double getEff2() const;
    bool isAlive() const;

private:

    double x; // trait value
    double eff1; // feeding efficiency on resource 1
    double eff2; // feeding efficiency on resource 2
    bool alive; // is the individual alive?

};

#endif