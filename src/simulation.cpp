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
		// - [OK] Time steps
		// - [OK] Individual habitat at each time step
		// - [OK] Individual trait value at each time step
		// - [OK] Individual total fitness at each time step
		// - [OK] Individual choice at each feeding round at each time step
		// - [OK] Individual position in the queue (index) at each feeding round at each time step
		// - [OK] Individual realized fitness at each feeding round at each time step
		// - [OK] Individual expected fitness difference at each feeding round at each time step
		// - [OK] Number of individuals in each habitat at each time step
		// - [OK] Mean trait value in each habitat at each time step
		// - [OK] Number of individuals on each resource in each habitat at each feeding round at each time step
		// - [OK] Mean trait value on each resource in each habitat at each feeding round at each time step
		// - [OK] A statistic for ecological isolation between ecotypes at each time step
		// - A statistic for spatial isolation between ecotypes at each time step

		// Which variables to save
        std::vector<std::string> filenames = { 
			
			"time", "individualHabitat", "individualTraitValue", "individualTotalFitness",
			"individualChoice", "individualIndex", "individualRealizedFitness",
			"individualExpectedFitnessDifference", "habitatCensus", "habitatMeanTraitValue",
			"resourceCensus", "resourceMeanTraitValue", "ecologicalIsolation"
		
		};

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
        int timeFile(-1), individualHabitatFile(-1), individualTraitValueFile(-1),
		individualTotalFitnessFile(-1), individualChoiceFile(-1), individualIndexFile(-1),
		individualRealizedFitnessFile(-1), individualExpectedFitnessDifference(-1),
		habitatCensusFile(-1), habitatMeanTraitValueFile(-1), resourceCensusFile(-1),
		resourceMeanTraitValueFile(-1), ecologicalIsolationFile(-1);
        for (size_t f = 0u; f < filenames.size(); ++f) {

            const std::string filename = filenames[f];

            if (filename == "time") timeFile = f;
			else if (filename == "individualHabitat") individualHabitatFile = f;
			else if (filename == "individualTraitValue") individualTraitValueFile = f;
			else if (filename == "individualTotalFitness") individualTotalFitnessFile = f;
			else if (filename == "individualChoice") individualChoiceFile = f;
			else if (filename == "individualIndex") individualIndexFile = f;
			else if (filename == "individualRealizedFitness") individualRealizedFitnessFile = f;
			else if (filename == "individualExpectedFitnessDifference") individualExpectedFitnessDifference = f;
			else if (filename == "habitatCensus") habitatCensusFile = f;
			else if (filename == "habitatMeanTraitValue") habitatMeanTraitValueFile = f;
			else if (filename == "resourceCensus") resourceCensusFile = f;
			else if (filename == "resourceMeanTraitValue") resourceMeanTraitValueFile = f;
			else if (filename == "ecologicalIsolation") ecologicalIsolationFile = f;
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
                outfiles[timeFile]->write((char *) &t_, sizeof(double));

            }

			// Save the number of individuals in each habitat if needed
			if (timetosave && habitatCensusFile >= 0) {

				// Count the number of individuals in each habitat
				std::vector<size_t> n(2u, 0u);
				for (size_t i = 0u; i < pop.size(); ++i) ++n[pop[i].getHabitat()];

				// Write to file
                const double n1_ = static_cast<double>(n[0u]);
				const double n2_ = static_cast<double>(n[1u]);
                outfiles[habitatCensusFile]->write((char *) &n1_, sizeof(double));
				outfiles[habitatCensusFile]->write((char *) &n2_, sizeof(double));

            }

			// Save the mean trait value in each habitat if needed
			if (timetosave && habitatMeanTraitValueFile >= 0) {

				// Compute the mean trait value in each habitat
				std::vector<size_t> n(2u, 0u);
				std::vector<double> meanx(2u, 0.0);
				for (size_t i = 0u; i < pop.size(); ++i) {

					++n[pop[i].getHabitat()];
					meanx[pop[i].getHabitat()] += pop[i].getX();

				}
				meanx[0u] /= n[0u];
				meanx[1u] /= n[1u];

				// Write to file
				outfiles[habitatMeanTraitValueFile]->write((char *) &meanx[0u], sizeof(double));
				outfiles[habitatMeanTraitValueFile]->write((char *) &meanx[1u], sizeof(double));

			}

			// Save a ecological isolation statistic if needed
			if (timetosave && ecologicalIsolationFile >= 0) {

				// Initialize mean trait value
				double meanx = 0.0;

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Record trait value
					const double x = pop[i].getX();

					// Update the mean trait value
					meanx += x;

				}

				// Finish the calculation of the mean trait value
				meanx /= pop.size();

				// Initialize vectors of numbers, sums and sums of squares of individual trait values in each ecotype,
				// with one extra slot for the total population
				std::vector<size_t> n(3u, 0u);
				std::vector<double> sumx(3u, 0.0);
				std::vector<double> ssqx(3u, 0.0);

				// Loop again...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Record trait value
					const double x = pop[i].getX();

					// Identify ecotype
					const bool ecotype = x > meanx;

					// Update the number of individuals in that ecotype and in the total population
					++n[ecotype];
					++n[2u];

					// Update the sum of squares of individual traits in that ecotype and in the total population
					ssqx[ecotype] += utl::sqr(x);
					ssqx[2u] += utl::sqr(x);

					// Update the sum of individual traits in that ecotype and in the total population
					sumx[ecotype] += x;
					sumx[2u] += x;

				}

				// Initialize a vector for the variance in trait value in each ecotype and in the total population
				std::vector<double> varx(3u, 0.0);

				// For each ecotype (plus the whole population)...
				for (size_t j = 0u; j < 3u; ++j) {
					
					// If thre are individuals in the current ecotype...
					if (n[j] > 0u) {

						// Compute variance
						varx[j] = ssqx[j] / n[j] - utl::sqr(sumx[j] / n[j]);

					}

					// Check that the variance is positive
					assert(varx[j] >= 0.0);

				}

				// Compute ecological isolation
				const double EI = 1.0 - (n[0u] * varx[0u] + n[1u] * varx[1u]) / (n[2u] * varx[2u]);

				// Write to file
				outfiles[ecologicalIsolationFile]->write((char *) &EI, sizeof(double));

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

				// Initialize numbers of individuals feeding on each resource
				size_t n1 = 0.0, n2 = 0.0;

				// Initialize sums of trait values of individuals feeding on each resource
				double meanx1 = 0.0, meanx2 = 0.0; 

				// For each individual...
				for (size_t i = 0; i < pop.size(); ++i) {

					// Save individual habitats if needed
					if (timetosave && individualHabitatFile >= 0) {

						const double habitat_ = static_cast<double>(pop[i].getHabitat());
						outfiles[individualHabitatFile]->write((char *) &habitat_, sizeof(double));

                	}

					// Read individual trait value
					const double x = pop[i].getX();

					// Save individual trait values if needed
					if (timetosave && individualTraitValueFile >= 0) {

						outfiles[individualTraitValueFile]->write((char *) &x, sizeof(double));

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

					// Save individual expected fitness difference if needed
					if (timetosave && individualExpectedFitnessDifference >= 0) {

						const double diff = fit2 - fit1;
						outfiles[individualExpectedFitnessDifference]->write((char *) &diff, sizeof(double));

                	}

					// Resource choice
					pop[i].setChoice(fit2 > fit1, pars.beta);

					// Read the choice that was made
					const bool choice = pop[i].getChoice();

					// Save individual choice if needed
					if (timetosave && individualChoiceFile >= 0) {

						const double choice_ = static_cast<double>(choice);
						outfiles[individualChoiceFile]->write((char *) &choice_, sizeof(double));

                	}

					// Update cumulative feeding efficiencies depending on what resource has been chosen
					if (choice) sumeff2 += eff2; else sumeff1 += eff1;

					// Update the number of individuals feeding on each resource
					if (choice) ++n2; else ++n1;

					// Update the sum of trait values of individuals feeding on each resource
					if (choice) meanx2 += x; else meanx1 += x;

					// Save individual index if needed
					if (timetosave && individualIndexFile >= 0) {

						const double index_ = static_cast<double>(pop[i].getIndex());
						outfiles[individualIndexFile]->write((char *) &index_, sizeof(double));

                	}
				}

				// Save the number of individuals feeding on each resource if needed
				if (timetosave && resourceCensusFile >= 0) {

					const double n1_ = static_cast<double>(n1);
					const double n2_ = static_cast<double>(n2);
					outfiles[resourceCensusFile]->write((char *) &n1_, sizeof(double));
					outfiles[resourceCensusFile]->write((char *) &n2_, sizeof(double));

                }

				// Save the mean trait value of individuals feeding on each resource if needed
				if (timetosave && resourceMeanTraitValueFile >= 0) {

					// Turn sums of trait values into means
					meanx1 /= n1;
					meanx2 /= n2;

					// Write to file
					outfiles[resourceMeanTraitValueFile]->write((char *) &meanx1, sizeof(double));
					outfiles[resourceMeanTraitValueFile]->write((char *) &meanx2, sizeof(double));

                }

				// For each individual...
				for (size_t i = 0u; i < pop.size(); ++i) {

					// Which resource was chosen?
					const bool choice = pop[i].getChoice();

					// Corresponding values
					const double eff = choice ? pop[i].getEff2() : pop[i].getEff1();
					const double res = choice ? pars.res2 : pars.res1;
					const double sumeff = choice ? sumeff2 : sumeff1;

					// Compute realized fitness on the chosen resource
					const double fit = res * eff * (sumeff + 1.0 / pars.delta - 1.0);

					// Check that the fitness is above zero
					assert(fit >= 0.0);

					// Save individual realized fitness if needed
					if (timetosave && individualRealizedFitnessFile >= 0) {

						outfiles[individualRealizedFitnessFile]->write((char *) &fit, sizeof(double));

                	}

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

			// For each adult individual...
			for (size_t i = 0u; i < pars.popsize; ++i) {
				
				// Record the final fitness if needed
				if (timetosave && individualTotalFitnessFile >= 0) {

					outfiles[individualTotalFitnessFile]->write((char *) &fitnesses[i], sizeof(double));

                }

				// Kill that individual
				pop[i].kill();

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
