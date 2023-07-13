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

		// Parameters
		const size_t popsize = 10u; // fixed population size
		const double tradeoff = 1.0; // resouce utilization tradeoff
		const double delta = 1.0; // resource discovery rate
		const double res1 = 1.0;
		const double res2 = 1.0;

		// Create a population of individuals
		std::vector<Individual> pop(popsize, { tradeoff });

		// Loop through time steps
		for (size_t t = 0u; t <= 5u; ++t) {

			std::cout << t << std::endl;
			
			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			// Initialize sums of feeding efficiencies for each resource
			double sumeff1, sumeff2 = 0.0;

			// For each individual...
			for (size_t i = 0u; i < pop.size(); ++i) {

				// Update the sums of feeding efficiencies
				sumeff1 += pop[i].getEff1();
				sumeff2 += pop[i].getEff2();

			}

			// For each individual in the population...
			for (size_t i = 0u; i < pop.size(); ++i) {

				const double eff1 = pop[i].getEff1();
				const double eff2 = pop[i].getEff2();

				const double fit1 = res1 * eff1 / (sumeff1 + 1.0 / delta - 1.0);
				const double fit2 = res2 * eff2 / (sumeff2 + 1.0 / delta - 1.0);

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
