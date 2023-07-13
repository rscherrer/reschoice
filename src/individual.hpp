#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

class Individual {

public:

    Individual();

    void kill();

    double getX() const;
    bool isAlive() const;

private:

    double x; // trait value
    bool alive; // is the individual alive?

};

#endif