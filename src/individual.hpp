#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

#include "utilities.hpp"
#include "random.hpp"
#include <cmath>
#include <cassert>

class Individual {

public:

    Individual(const double&);

    void kill();
    void setChoice(const bool&, const double&);
    void mutate(const double&, const double&);
    void develop(const double&);
    void isBorn();

    double getX() const;
    double getEff1() const;
    double getEff2() const;
    bool getHabitat() const;
    bool getEcotype() const;
    bool getChoice() const;
    size_t getIndex() const;
    bool isAlive() const;

private:

    double x; // trait value
    double eff1; // feeding efficiency on resource 1
    double eff2; // feeding efficiency on resource 2
    bool habitat; // habitat where the individual lives
    bool ecotype; // what resource is the individual more adapted to relative to the pop average
    bool choice; // which resource is chosen?
    size_t index; // individual identifier (to keep track despite shuffling)
    bool alive; // is the individual alive?

};

#endif