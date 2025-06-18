// This is the main simulation script. The core of the program has been
// encapsulated into a function called doMain(), so its proper behavior
// can be tested in different use cases in our tests.

#include "MAIN.hpp"

// Function to return a list of valid outputs
std::vector<std::string> valid() {

    // Return a list of valid outputs
    return {

        "time",                                     // 0    time steps
        "resourceCensus",                           // 1    number of individuals feeding on each resource at each feeding round in each habitat
        "resourceMeanTraitValue",                   // 2    mean trait value of individuals feeding on each resource at each feeding round in each habitat
        "individualExpectedFitnessDifference",      // 3    expected fitness difference between the resources for each individual at each feeding round
        "individualChoice",                         // 4    resource choice made by each individual at each feeding round
        "individualRealizedFitness",                // 5    realized fitness gain of each individual after each feeding round
		"individualRank",                           // 6    position of each individual in the feeding queue at each feeding round
        "individualHabitat",                        // 7    habitat of each individual
        "individualTraitValue",                     // 8    trait value of each individual
        "individualTotalFitness",                   // 9    total fitness of each individual after all feeding rounds
        "individualEcotype",                        // 10   ecotype of each individual (relative to population average)
		"habitatCensus",                            // 11   number of individuals in each habitat
        "habitatMeanTraitValue",                    // 12   mean trait value in each habitat
		"ecologicalIsolation",                      // 13   ecological isolation statistic
        "spatialIsolation",                         // 14   spatial isolation statistic
        "traitStandardDeviation"                    // 15   trait standard deviation in the population

    };

}

// Simulation function
void doMain(const std::vector<std::string> &args) {

	// args: program arguments

	// Error if two many arguments
    if (args.size() > 2u) 
        throw std::runtime_error("Too many arguments provided");

	// Parameter file
    const std::string parfile = args.size() == 2u ? args[1u] : "";

	// Create parameters (from file if needed)
    const Parameters pars(parfile);

	// Verbose
    if (args.size() == 2u) std::cout << "Parameters read in succesfully\n";

	// Seed the random number generator
    rnd::rng.seed(pars.seed);

	// Check
    pars.check();

	// Save parameters if necessary
    if (pars.savepars) pars.save("paramlog.txt");

	// Create a printer
    Printer print(valid(), pars.memsave);

	// If needed...
    if (pars.savedat && pars.choose) {
        
        // Read outputs to save
        print.read("whattosave.txt");

        // Verbose
        std::cout << "Requested output(s) read in succesfully\n";

    }

	// If needed...
    if (pars.savedat) {
        
        // Open the buffers
        print.open();

        // Verbose
        std::cout << "Output files open succesfully\n";

    }

	// Create a population of individuals
    Population pop(pars);
	
	// Verbose
    std::cout << "Simulation started\n";

	// At each time step...
    while (pop.keepon()) {

        // Check that parameters are still valid 
        pop.check();

        // Go through the life cycle of the population
        pop.cycle(print);

        // Move on
        pop.moveon();

    }

    // End of simulation
    std::cout << "Simulation ended\n";

    // Close the streams to output files
    if (pars.savedat) print.close();
	
}
