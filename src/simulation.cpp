// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.hpp"

int simulate(const std::vector<std::string> &args) {

	try
    {

		// Fixed population size
		size_t popsize = 10u;

		// Create a population of individuals
		std::vector<Individual> pop(popsize);

		// Loop through time steps
		for (size_t t = 0u; t <= 5u; ++t) {

			std::cout << t << std::endl;

			// Loop through individuals to be made
			for (size_t i = 0u; i < popsize; ++i) {

				// Add offspring to the population
				pop.push_back(pop[i]);				

			}
		}

        return 0;

    }
    catch (const std::exception& err)
    {
        std::cerr << "Exception: " << err.what() << '\n';
    }
    catch (const char* err)
    {
        std::cerr << "Exception: " << err << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown Exception\n";
    }
    return 1;

}
