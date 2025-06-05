#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

#include "utilities.hpp"
#include "random.hpp"
#include <cmath>
#include <cassert>

// Accessory functions
double calcBaselineProb(const double&, const double&, const double&);
double calcProbBest(const double&, const double&);

class Individual {

public:

    // Constructor
    Individual(const double&, const double&);

    // Setters
    void makeChoice(const double&, const double&, const double&, const double& = 0.0, const double& = 1.0, const double& = 1.0);
    void setDiff(const double&);
    void mutate(const double&, const double&);
    void develop(const double&);
    void disperse();
    void setEcotype(const double&);
    void setRank(const size_t&); 

    // Getters
    double getX() const;
    double getEff1() const;
    double getEff2() const;
    double getDiff() const;
    bool getHabitat() const;
    bool getEcotype() const;
    bool getChoice() const;
    size_t getRank() const;

private:

    double x;       // trait value
    double eff1;    // feeding efficiency on resource 1
    double eff2;    // feeding efficiency on resource 2
    double diff;    // expected fitness difference
    bool habitat;   // habitat where the individual lives
    bool ecotype;   // what resource is the individual more adapted to relative to the pop average
    bool choice;    // which resource is chosen?
    size_t rank;    // individual position in a queue during a feeding round

};

#endif