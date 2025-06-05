#ifndef RESCHOICE_POPULATION_HPP
#define RESCHOICE_POPULATION_HPP

// This is the header for the Population class, which encapsulates
// a vector of Individuals and makes them go through each generation
// of the simulation.

#include "printer.hpp"
#include "parameters.hpp"
#include "individual.hpp"
#include "utilities.hpp"

namespace pop {

    // Accessory functions
    double resources(const double&, const double&, const double&);
    double fitness(const double&, const double&, const double&, const size_t&);

}

namespace stat {

    // Compute statistics
    double ei(const std::vector<std::vector<size_t> >&, const std::vector<std::vector<double> >&, const std::vector<std::vector<double> >&);
	double si(const std::vector<std::vector<size_t> >&);

}

class Population {

public:

    // Constructor
    Population(const Parameters&);

    // Setters
    void cycle(Printer&);
    void moveon();

    // Checkers
    void check() const;
    void show() const;

    // Function to tell whether to keep on simulating
    bool keepon() const { return time <= tend; };
    
    // Other getters
    size_t getTime() const { return time; };
    size_t getHabitat(const size_t &i) const { assert(i < individuals->size()); return (*individuals)[i].getHabitat(); };

private:

    // The individuals
    std::unique_ptr<std::vector<Individual> > individuals;
    std::unique_ptr<std::vector<Individual> > newborns;

    // Parameters
    size_t popsize;      // fixed population size
    double tradeoff;     // resouce utilization tradeoff    
    double alpha;        // resource abundance weight
    double beta;         // resource choice accuracy
    double delta;        // resource discovery rate
    double hsymmetry;    // habitat symmetry parameter
	size_t nrounds;      // number of feeding rounds
	double mutrate;      // mutation rate
	double mutsdev;      // mutational standard deviation
    double dispersal;    // dispersal rate
    size_t tend;         // simulation time
    size_t tsave;        // recording time

    // Resources in each habitat
    std::vector<std::vector<double> > resources;

    // Distribution of mutations
    std::normal_distribution<double> sampleMutation;

    // Variables
    size_t time;
    
};

#endif