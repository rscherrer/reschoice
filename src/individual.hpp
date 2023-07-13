#ifndef RESCHOICE_INDIVIDUAL_HPP
#define RESCHOICE_INDIVIDUAL_HPP

// This is the header for the Individual class.

class Individual {

public:

    Individual();

    void kill();

    bool isAlive() const;

private:

    bool alive; // is the individual alive?

};

#endif