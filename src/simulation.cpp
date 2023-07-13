// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.hpp"

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

int simulate(const std::vector<std::string> &args) {

	try
    {

		// Fixed population size
		const size_t popsize = 10u;

		// Create a population of individuals
		std::vector<Individual> pop(popsize);

		// Loop through time steps
		for (size_t t = 0u; t <= 5u; ++t) {

			std::cout << t << std::endl;
			
			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			// For each individual in the population...
			for (size_t i = 0u; i < pop.size(); ++i) {

				// Assign it a fitness value
				fitnesses[i] = 1.0;

			}

			// Create a distribution to sample parents from proportionately to fitness
			auto sampleParent = rnd::discrete(fitnesses.begin(), fitnesses.end());

			// For each individual to be born...
			for (size_t i = 0u; i < popsize; ++i) {

				// Sample parent of the current offspring (with replacement)
				const size_t j = sampleParent(rnd::rng);

				// Add offspring to the population
				pop.push_back(pop[j]);

			}

			// Kill every adult
			for (size_t i = 0u; i < popsize; ++i) pop[i].kill();

			// Remove dead individuals
            auto it = std::remove_if(pop.begin(), pop.end(), burry);
            pop.erase(it, pop.end());
            pop.shrink_to_fit();

			// Make sure population size has not changed
			assert(pop.size() == popsize);

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
