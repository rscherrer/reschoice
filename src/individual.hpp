#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

#include "utilities.hpp"
#include "random.hpp"
#include <cmath>
#include <cassert>

class Individual {

public:

    Individual(const double&, const double&);

    void kill();
    void makeChoice(const double&, const double&, const double&);
    void setDiff(const double&);
    void mutate(const double&, const double&);
    void develop(const double&);
    void disperse();
    void isBorn();
    void setEcotype(const double&);
    void setRank(const size_t&); 

    double getX() const;
    double getEff1() const;
    double getEff2() const;
    double getDiff() const;
    bool getHabitat() const;
    bool getEcotype() const;
    bool getChoice() const;
    size_t getRank() const;
    bool isAlive() const;

private:

    double x; // trait value
    double eff1; // feeding efficiency on resource 1
    double eff2; // feeding efficiency on resource 2
    double diff; // expected fitness difference
    bool habitat; // habitat where the individual lives
    bool ecotype; // what resource is the individual more adapted to relative to the pop average
    bool choice; // which resource is chosen?
    size_t rank; // individual position in a queue during a feeding round
    bool alive; // is the individual alive?

};

#endif