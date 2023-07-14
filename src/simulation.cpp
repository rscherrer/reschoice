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

		// Create a default parameter set
		Parameters pars;

		// Error if two many arguments
        if (args.size() > 2u) throw std::runtime_error("Two many arguments provided.");

		// Read parameters from a file if supplied
        if (args.size() == 2u) pars.read(args[1u]);

		// Save parameters if necessary
        if (pars.savepars) pars.save();

		// Redirect output to log file if needed
        if (pars.savelog) pars.savelog = std::freopen("log.txt", "w", stdout);

		// Create a vector of output file streams (using smart pointers)
        std::vector<std::shared_ptr<std::ofstream> > outfiles;

		// What variables do we save in this study?
		// - Time steps
		// - Individual habitat at each time step
		// - Individual trait value at each time step
		// - Individual realized fitness at each time step
		// - Individual choice at each feeding round at each time step
		// - Individual position in the queue at each feeding round at each time step
		// - Individual realized fitness at each feeding round at each time step
		// - Individual expected fitness at each feeding round at each time step
		// - Number of individuals in each habitat at each time step
		// - Mean trait value in each habitat at each time step
		// - Number of individuals on each resource in each habitat at each feeding round at each time step
		// - Mean trait value on each resource in each habitat at each feeding round at each time step
		// - A statistic for phenotypic divergence in the population at each time step
		// - A statistic for spatial divergence in the poopulation at each time step

		// Which variables to save
        std::vector<std::string> filenames = { "time", "individualHabitat" };

		// Update the list of which variables to save if needed...
        if (pars.choose) {

            // Read file where those are provided
            std::ifstream infile("whattosave.txt");
            if (!infile.is_open())
                throw std::runtime_error("Could not read input file whattosave.txt");
            std::vector<std::string> newfilenames;
            std::string input;
            while (infile >> input) newfilenames.push_back(input);
            stf::check(newfilenames, filenames);
            filenames.reserve(newfilenames.size());
            filenames.resize(newfilenames.size());
            for (size_t f = 0u; f < newfilenames.size(); ++f)
                filenames[f] = newfilenames[f];

        }

		// Open the file streams
        stf::open(outfiles, filenames);

		// Set up flags for which data to save
        int timeFile(-1), individualHabitatFile(-1);
        for (size_t f = 0u; f < filenames.size(); ++f) {

            const std::string filename = filenames[f];

            if (filename == "time") timeFile = f;
			else if (filename == "individualHabitat") individualHabitatFile = f;
            else throw std::runtime_error("Invalid output requested in whattosave.txt");

        }

		// Distribution of mutational deviations (set up here for speed)
		auto sampleMutation = rnd::normal(0.0, pars.mutsdev);

		// Create a population of individuals
		std::vector<Individual> pop(pars.popsize, { pars.tradeoff });

		// Screen output
		std::cout << "Simulation started.\n";

		// Loop through time steps
		for (size_t t = 0u; t <= pars.tend; ++t) {

			// Verbose if needed
			if (pars.talkative) std::cout << "t = " << t << '\n';

			// Flag to know if it is time to save some data
            const bool timetosave = t % pars.tsave == 0u;

			// Save time if needed
            if (timetosave && timeFile >= 0) {
                const double t_ = static_cast<double>(t);
                outfiles[timeFile]->write((char *) &(t_), sizeof(double));
            }
			
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
			// expected one but now with sums of feeding efficiencies being computed once everybody
			// has chosen. The total fitness of an individual is the sum of all the food obtained
			// throughout all feeding rounds.

			// Initialize a vector of fitnesses
			std::vector<double> fitnesses(pop.size());

			// For each feeding round...
			for (size_t j = 0u; j < pars.nrounds; ++j) {

				// Individuals must be taken in random order
				std::shuffle(pop.begin(), pop.end(), rnd::rng);

				// Initialize cumulative feeding efficiencies
				double sumeff1 = 0.0, sumeff2 = 0.0;

				// For each individual...
				for (size_t i = 0; i < pop.size(); ++i) {

					// Save individual habitats if needed
					if (timetosave && individualHabitatFile >= 0) {

						const double habitat_ = static_cast<double>(pop[i].getHabitat());
						outfiles[individualHabitatFile]->write((char *) &habitat_, sizeof(double));

                	}

					// Get feedding efficiency on each resource
					const double eff1 = pop[i].getEff1();
					const double eff2 = pop[i].getEff2();

					// Compute expected fitness on each resource
					const double fit1 = pars.res1 * eff1 * (sumeff1 + 1.0 / pars.delta - 1.0);
					const double fit2 = pars.res2 * eff2 * (sumeff2 + 1.0 / pars.delta - 1.0);

					// Check that expected fitnesses are above zero
					assert(fit1 >= 0.0);
					assert(fit2 >= 0.0);

					// Resource choice
					pop[i].setChoice(fit2 > fit1, pars.beta);
					
					// Update cumulative feeding efficiencies depending on what resource has been chosen
					if (pop[i].getChoice()) sumeff2 += eff2; else sumeff1 += eff1;

				}

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Which resource was chosen?
					const bool choice = pop[i].getChoice();

					// Corresponding values
					const double eff = choice ? pop[i].getEff2() : pop[i].getEff1();
					const double res = choice ? pars.res2 : pars.res1;
					const double sumeff = choice ? sumeff2 : sumeff1;

					// Compute realized fitness on each resource
					const double fit = res * eff * (sumeff + 1.0 / pars.delta - 1.0);

					// Check that the fitness is above zero
					assert(fit >= 0.0);

					// Add obtained food to the vector of fitnesses
					fitnesses[i] += fit;

				}
			}

			// Create a distribution to sample parents from proportionately to fitness
			auto sampleParent = rnd::discrete(fitnesses.begin(), fitnesses.end());

			// For each individual to be born...
			for (size_t i = 0u; i < pars.popsize; ++i) {

				// Sample parent of the current offspring (with replacement)
				const size_t j = sampleParent(rnd::rng);

				// Add offspring to the population
				pop.push_back(pop[j]);

				// Mutate offspring if needed
				if (rnd::bernoulli(pars.mutrate)(rnd::rng)) 
					pop.back().mutate(sampleMutation(rnd::rng), pars.tradeoff);

			}

			// Kill every adult
			for (size_t i = 0u; i < pars.popsize; ++i) pop[i].kill();

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
        stf::close(outfiles);

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
