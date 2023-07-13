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
			
			// There are multiple feeding rounds.
			// Every feeding round, individuals are taken in random order.
			// Eech individual assesses its expected fitness if feeding on each resource.
			// That expected fitness depends on the amount of resources present, the discovery rate, 
			// its own affinity for that resource and the cumulative affinity of all individuals
			// feeding on that resource (i.e. having chosen that resource).
			// Of those, the amount of resource and the discovery rate remain constant (resources
			// are replenished every round and do not go down until everyone has made a choice),
			// and the feeding efficiency only depends on the individual's trait value x.
			// The cumulative affinity does change as we loop through individuals within one round
			// though. Every time an individual makes a choice, that value changes, so we need
			// to keep track of it. Once all individuals have chosen, the resources is split.
			// Well, only the amount of resources that was discovered (that needs to be calculated
			// depending on the discovery rate and individual choices). And it is split proportionately
			// to the realized fitness of the individuals, which uses the same formula as the
			// expected one but now with sums of feeding efficiencies being computed once everybody
			// has chosen.

			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			////////////////////////////

			// One round for now

			// Initialize cumulative feeding efficiencies
			double sumeff1, sumeff2 = 0.0;

			// For each individual...
			for (size_t i = 0; i < pop.size(); ++i) {

				// Get feedding efficiency on each resource
				const double eff1 = pop[i].getEff1();
				const double eff2 = pop[i].getEff2();

				// Compute expected fitness on each resource
				const double fit1 = res1 * eff1 * (sumeff1 + 1.0 / delta - 1.0);
				const double fit2 = res2 * eff2 * (sumeff2 + 1.0 / delta - 1.0);

				// Resource choice
				pop[i].setChoice(fit2 > fit1);
				
				// Update cumulative feeding efficiencies depending on what resource has been chosen
				if (pop[i].getChoice()) sumeff2 += eff2; else sumeff1 += eff1;

			}

			for (size_t i = 0u; i < pop.size(); ++i) {

				// Which resource was chosen?
				const bool choice = pop[i].getChoice();

				// Corresponding values
				const double eff = choice ? pop[i].getEff2() : pop[i].getEff1();
				const double res = choice ? res2 : res1;
				const double sumeff = choice ? sumeff2 : sumeff1;

				// Compute realized fitness on each resource
				const double fit = res * eff * (sumeff + 1.0 / delta - 1.0);

				// Add it to the vector of fitnesses
				fitnesses[i] = fit;

			}

			////////////////////////////

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
