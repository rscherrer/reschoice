// This is the main simulation script. The core of the program has been
// encapsulated into a function called simulate(), so its proper behavior
// can be tested in different use cases in our tests.

#include "simulation.hpp"

// Lambda for removing dead individuals
auto burry = [](Individual ind) -> bool
{
    return !ind.isAlive();
};

// Function to compute the amount of resource discovered by a group of feeders
double calcResourceDiscovered(const double &rtotal, const double &delta, const double &sumeffs) {

	// rtotal: total amount of resource in the habitat
	// delta: resource discovery rate
	// sumeffs: cumulative consumption rate of the feeders

	// Check
	assert(rtotal >= 0.0);
	assert(delta >= 0.0);
	assert(sumeffs >= 0.0);

	// Early exit in case of zero (avoid unnecessary exponential)
	if (!sumeffs || !delta || !rtotal) return 0.0;

	// Compute the saturating curve
	return rtotal * (1.0 - exp(-delta * sumeffs));

}

// Function to compute estimated fitness of an individual
double calcFitness(const double &rdiscov, const double &eff, const double &sumeffs, const size_t &n) {

	// rdiscov: amount of resource discovered
	// eff: consumption efficiency of the focal individual
	// sumeffs: cumulative consumption rate of all the feeders (incl. focal individual)
	// n: number of feeders (only used if sumeffs is zero)

	// Check
	assert(rdiscov >= 0.0);
	assert(eff >= 0.0);
	assert(sumeffs >= eff);

	// Special case if cumulative consumption rate (denominator) is zero
	if (!sumeffs) return n ? rdiscov * 1.0 / n : 0.0;

	// Note: if there are feeders but all with consumption rate of zero, they
	// split the resource equally. Fitness is only zero if the reason why the
	// cumulative rate is zero is that there are no feeders.

	// Compute fitness
	return rdiscov * eff / sumeffs;

}

// Simulation function
int simulate(const std::vector<std::string> &args) {

	try
    {

		// Create a default parameter set
		Parameters pars;

		// Error if two many arguments
        if (args.size() > 2u) throw std::runtime_error("Two many arguments provided.");

		// Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

		// Save parameters if necessary
        if (pars.savepars) pars.save();

		// Redirect output to log file if needed
        if (pars.savelog) pars.savelog = std::freopen("screenlog.txt", "w", stdout);

		// Set up a printer
		Printer print(pars.whattosave);

		// Distribution of mutational deviations (set up here for speed)
		auto sampleMutation = rnd::normal(0.0, pars.mutsdev);

		// Create a population of individuals
		std::vector<Individual> pop(pars.popsize, { pars.xstart, pars.tradeoff });

		// Make sure the container can contain twice the population size
		pop.reserve(2u * pop.size());

		// Create a matrix of resource concentrations
		std::vector<std::vector<double> > resources({{0.0, 0.0}, {0.0, 0.0}});
		for (size_t i = 0u; i < 2u; ++i)
			for (size_t j = 0u; j < 2u; ++j)
				resources[i][j] = i == j ? 1.0 : pars.hsymmetry;

		// Screen output
		std::cout << "Simulation started.\n";

		// Loop through time steps
		for (size_t t = 0u; t <= pars.tend; ++t) {

			// Verbose if needed
			if (pars.talkative) std::cout << "t = " << t << '\n';

			// Flag to know if it is time to save some data
            const bool timeToSave = t % pars.tsave == 0u;

			// Save time if needed
			if (timeToSave) print.save(t, 0u);

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
			// to keep track of it. Once all individuals have chosen, the resources is split
			// (well, only the amount of resources that was discovered). And it is split proportionately
			// to the realized fitness of the individuals, which uses the same formula as the
			// expected one but now with sums of feeding efficiencies being computed with everybody's values
			// throughout all feeding rounds.

			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			// Initialize a vector of consecutive indices
			std::vector<size_t> indices(pop.size());
			std::iota(indices.begin(), indices.end(), 0u);

			// For each feeding round...
			for (size_t j = 0u; j < pars.nrounds; ++j) {

				// Shuffle indices to take individuals in random order
				std::shuffle(indices.begin(), indices.end(), rnd::rng);

				// Initialize cumulative feeding efficiencies in each habitat on each resource
				std::vector<std::vector<double> > sumeffs({{0.0, 0.0}, {0.0, 0.0}});

				// Initialize other useful containers
				std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
				std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}}); 

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Respect random order
					const size_t ii = indices[i];

					// Assign a rank in the queue to the individual
					pop[ii].setRank(i);

					// Read individual properties
					const double x = pop[ii].getX();
					const bool habitat = pop[ii].getHabitat();

					// Get feeding efficiency on each resource
					const std::vector<double> effs({ pop[ii].getEff1(), pop[ii].getEff2() });

					// Compute the cumulative consumption rates so far on each resource (incl. focal individual)		
					const double cumul1 = sumeffs[habitat][0u] + effs[0u];
					const double cumul2 = sumeffs[habitat][1u] + effs[1u];

					// Check
					assert(cumul1 >= effs[0u]);
					assert(cumul2 >= effs[1u]);

					// Compute the amount of resource discovered for each resource
					const double discov1 = calcResourceDiscovered(resources[habitat][0u], pars.delta, cumul1);
					const double discov2 = calcResourceDiscovered(resources[habitat][1u], pars.delta, cumul2);

					// Check
					assert(discov1 >= 0.0);
					assert(discov2 >= 0.0);

					// Check that the resource discovery function is indeed saturating
					assert(discov1 <= resources[habitat][0u]);
					assert(discov2 <= resources[habitat][1u]);

					// Compute expected fitness on each resource (special case when denominator is zero)
					const double fit1 = calcFitness(discov1, effs[0u], cumul1, n[habitat][0u] + 1u);
					const double fit2 = calcFitness(discov2, effs[1u], cumul2, n[habitat][1u] + 1u);

					// Check that expected fitnesses are above zero
					assert(fit1 >= 0.0);
					assert(fit2 >= 0.0);

					// Record expected fitness difference
					pop[ii].setDiff(fit2 - fit1);

					// Make the individual choose
					pop[ii].makeChoice(fit1, fit2, pars.alpha, pars.beta, resources[habitat][0u], resources[habitat][1u]);

					// Read the choice that was made
					const bool choice = pop[ii].getChoice();

					// Update cumulative feeding efficiencies depending on what resource has been chosen, in that habitat
					sumeffs[habitat][choice] += effs[choice];

					// Update other important statistics
					++n[habitat][choice];
					sumx[habitat][choice] += x;

				}

				// Save the number and mean trait values of individuals feeding on each resource in each habitat if needed
				if (timeToSave) {

					for (size_t i = 0u; i < 2u; ++i) {
						for (size_t k = 0u; k < 2u; ++k) {

							print.save(n[i][k], 1u);
							print.save(n[i][k] ? sumx[i][k] / n[i][k] : 0.0, 2u);

						}
					}
				}

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Read relevant individual properties
					const bool choice = pop[i].getChoice();
					const bool habitat = pop[i].getHabitat();
					const bool diff = pop[i].getDiff();
					const size_t rank = pop[i].getRank();

					// Corresponding feeding efficiency
					const double eff = choice ? pop[i].getEff2() : pop[i].getEff1();

					// Compute realized fitness on the chosen resource
					const double fit = resources[habitat][choice] * eff / (sumeffs[habitat][choice] + 1.0 / pars.delta - 1.0);

					// Check that the fitness is above zero
					assert(fit >= 0.0);

					// Add obtained food to the vector of fitnesses
					fitnesses[i] += fit;

					// Save a few individual properties if needed
					if (timeToSave) {
						
						print.save(diff, 3u);
						print.save(choice, 4u);
						print.save(fit, 5u);
						print.save(rank, 6u);

					}

					// Set individual ecotype relative to population average while we are looping through individuals
					if (!j) pop[i].setEcotype((sumx[0u][0u] + sumx[0u][1u] + sumx[1u][0u] + sumx[1u][1u]) / pop.size());

				}
			}

			// Create a distribution to sample parents from proportionately to fitness
			auto sampleParent = rnd::discrete(fitnesses.begin(), fitnesses.end());

			// Initialize useful containers for habitat- and ecotype-specific statistics
			std::vector<std::vector<size_t> > n({{0u, 0u}, {0u, 0u}});
			std::vector<std::vector<double> > sumx({{0.0, 0.0}, {0.0, 0.0}});
			std::vector<std::vector<double> > ssqx({{0.0, 0.0}, {0.0, 0.0}});

			// For each individual to be born...
			for (size_t i = 0u; i < pars.popsize; ++i) {

				// Sample parent of the current offspring (with replacement)
				const size_t j = sampleParent(rnd::rng);

				// Add offspring to the population by cloning
				pop.push_back(pop[j]);

				// Make sure it is alive (the adult may have been killed already)
				pop.back().isBorn(); 

				// Mutate offspring if needed
				if (rnd::bernoulli(pars.mutrate)(rnd::rng)) 
					pop.back().mutate(sampleMutation(rnd::rng), pars.tradeoff);

				// The offspring has a chance to disperse
				if (rnd::bernoulli(pars.dispersal)(rnd::rng))
					pop.back().disperse();

				// Kill an adult corresponding the the index of the current offspring
				pop[i].kill();

				// Get relevant individual metrics for that adult
				const double x = pop[i].getX();
				const bool habitat = pop[i].getHabitat();
				const bool ecotype = pop[i].getEcotype();

				// Update habitat- and ecotype-specific statistics
				++n[habitat][ecotype];
				sumx[habitat][ecotype] += x;
				ssqx[habitat][ecotype] += utl::sqr(x);

				// Save a few individual properties of that adult if needed
				if (timeToSave) {
					
					print.save(habitat, 7u);
					print.save(x, 8u);
					print.save(fitnesses[i], 9u);
					print.save(ecotype, 10u);

				}
			}

			// Save some population-level statistics if needed
			if (timeToSave) {

				// Save the number and mean trait values of individuals in each habitat
				for (size_t i = 0u; i < 2u; ++i) {

					const size_t n0 = n[i][0u] + n[i][1u];
					print.save(n0, 11u);
					print.save(n0 ? (sumx[i][0u] + sumx[i][1u]) / n0 : 0.0, 12u);

				}

				// Save ecological isolation
				print.save(als::getEI(n, sumx, ssqx), 13u);

				// Save spatial isolation
				print.save(als::getSI(n), 14u);

			}

			// Remove dead individuals
            auto it = std::remove_if(pop.begin(), pop.end(), burry);
            pop.erase(it, pop.end());
            pop.shrink_to_fit();

			// Make sure population size has not changed
			assert(pop.size() == pars.popsize);

		}

		// Screen output
		std::cout << "Simulation ended.\n";

		// Close the log file if needed
		if (pars.savelog) std::fclose(stdout);

		// Close output file streams
        print.close();

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
